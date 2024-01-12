#include "ServerListenerThread.h"

ServerListenerThread::ServerListenerThread(const QString& ipAddress, int port, QObject *parent)
    : QThread(parent), mIpAddress(ipAddress), mPort(port) {
    // ... constructor implementation ...
    emit loadingProgress(-1);
    drawSelected = false;
    connected = false;
    moveSelected = false;
    received_packet_num = 0;
}

int16_t convertTwosComplement(const std::string &binaryString) {
    // Ensure the string length is 16 (16-bit representation)
    if (binaryString.length() != 16) {
        // Handle error or return default value as needed
        return 0; // Default value (you can change this)
    }

    // Check if the most significant bit is 1 (indicating a negative number)
    bool isNegative = (binaryString[0] == '1');

    // Convert the binary string to a 16-bit integer
    int16_t value = 0;

    for (size_t i = 0; i < 16; ++i) {
        value <<= 1; // Shift the value left by 1

        // Add the current bit to the value
        if (binaryString[i] == '1') {
            value |= 1;
        }
    }

    // If it's a negative number, perform two's complement
    if (isNegative) {
        value = -(~value + 1); // Two's complement calculation
    }

    return value;
}

// double convertTwosComplementToDouble(const std::string &binaryString) {
//     // Ensure the string length is 16 (16-bit representation)
//     if (binaryString.length() != 16) {
//         // Handle error or return default value as needed
//         return 0; // Default value (you can change this)
//     }

//     // Check if the most significant bit is 1 (indicating a negative number)
//     bool isNegative = (binaryString[0] == '1');

//     // Convert the binary string to a 16-bit integer
//     double value = 0;

//     for (size_t i = 0; i < 16; ++i) {
//         value <<= 1; // Shift the value left by 1

//         // Add the current bit to the value
//         if (binaryString[i] == '1') {
//             value |= 1;
//         }
//     }

//     // If it's a negative number, perform two's complement
//     if (isNegative) {
//         value = -(~value + 1); // Two's complement calculation
//     }

//     return value;
// }


QByteArray intToTwosComplement(int value) {
    QByteArray result;

    // Ensure the size of the integer is 16 bits (2 bytes)
    if ((value < -32768) || (value > 32767)) {
        // Handle the case where the value is out of range for a signed 16-bit integer
        // You might want to return a default value, throw an error, or handle it as needed
        return result; // Returning an empty result for demonstration
    }

    // If the value is negative, get its two's complement representation
    if (value < 0) {
        // Compute the two's complement by adding 65536 (2^16) to the negative value
        value += 65536; // 2^16
    }

    // Store the integer as a 16-bit little-endian byte sequence
    result.append(static_cast<char>(value & 0xFF)); // Lower byte
    result.append(static_cast<char>((value >> 8) & 0xFF)); // Upper byte

    return result;
}

void ServerListenerThread::applyBorder(const QString filePath) {
    QImage image(filePath); // Load the image
    if (image.isNull()) {
        qDebug() << "Failed to load the image.";
        return; // Exit or handle the failure
    }

    // Define the size of the border (in pixels)
    int borderWidth = 10;

    // Create a painter to draw on the image
    QPainter painter(&image);
    painter.setPen(QPen(Qt::black, 4)); // Set the pen color to black and width to 4 pixels
    painter.setRenderHint(QPainter::Antialiasing); // Optional: Enable anti-aliasing for smoother lines

    // Draw a rectangle border around the image
    painter.drawRect(borderWidth, borderWidth, image.width() - 2 * borderWidth - 4, image.height() - 2 * borderWidth - 4);

    if (!image.save(filePath)) {
        qDebug() << "Failed to save the framed image.";
        return; // Exit or handle the failure
    }
}



void ServerListenerThread::run() {
    QDir dir;
    dir = dir.temp();
    dir.setPath(dir.path()+ "/cse396");

    qDebug() << "Run works";
    emit loadingProgress(-1);
    while (!isInterruptionRequested()) {
        tcpSocket = new QTcpSocket();
        tcpSocket->connectToHost(mIpAddress, mPort);
        if (!tcpSocket->waitForConnected(3000)) { // 3-second timeout for connection attempt
            qDebug() << "Error: Cannot connect to the server. Retrying in 3 seconds...";
            tcpSocket->close();
            delete tcpSocket;
            QThread::sleep(3); // Sleep for 3 seconds before next attempt
            continue;
        }

        qDebug() << "Connected to the server.";
        connected = true;
        QByteArray messageToSend;
        messageToSend.append('0');
        messageToSend.append(QString::fromStdString("Connection established").toUtf8());
        qDebug() << "Message created...";
        if (!messageToSend.isEmpty() && tcpSocket->state() == QAbstractSocket::ConnectedState) {
            tcpSocket->write(messageToSend); // Sending the message to the server
            qDebug()  << messageToSend << " is sent! (FIRST)";
            qDebug() << "Message sending...";
            //tcpSocket->waitForBytesWritten(); // Wait for the message to be sent (optional)
            qDebug() << "Message sent...";
        }
        emit loadingProgress(0);
        int calculatedValue = 0;
        int totalLineNumberForAnImage = 0;
        QByteArray jsonFile;
        QByteArray imageByteArray;
        //int chunkSize = 4096 - 1; // 4KB - 1
        int chunkSize = 1024 - 1; // 4KB - 1
        int bytesWritten = 0;
        while (!isInterruptionRequested()) {
            QByteArray receivedData;
            if (tcpSocket->waitForReadyRead(5000)) {
                receivedData = tcpSocket->readAll();
                //qDebug() <<"Received:" << receivedData;
                received_packet_num++;
                emit stats_received_packet_num(received_packet_num);
                emit stats_last_packet_size(receivedData.size());

                if (!receivedData.isEmpty()) {

                    char command = receivedData[0];
                    receivedData = receivedData.remove(0, 1); // Removes the first element
                    messageToSend.clear();
                    switch(command){
                    case '0': //Connection established
                    {
                        //Eğer yeni bir dosya kaydedildiyse burayı tetikle. Dosyayı çek ve gönder

                        if(drawSelected)
                        {
                            QString filePath = dir.path() + "/image.jpg";
                            //applyBorder(filePath);
                            QFile file(filePath);
                            if (!file.open(QIODevice::ReadOnly)) {
                                qDebug() << "Failed to open the image file.";
                                continue;
                            }

                            emit drawingStatus(true);
                            emit loadingProgress(0);
                            imageByteArray = file.readAll();
                            file.close();

                            if(bytesWritten < imageByteArray.size()){
                                int remainingBytes = imageByteArray.size() - bytesWritten;
                                int bytesToWrite = qMin(chunkSize, remainingBytes);
                                messageToSend.append(imageByteArray.mid(bytesWritten, bytesToWrite));
                                bytesWritten += bytesToWrite;
                            }

                            if (bytesWritten >= imageByteArray.size()){
                                messageToSend.push_front('2');
                                emit stages(2);
                                bytesWritten = 0;
                                imageByteArray.clear();
                                drawSelected  = false;
                            }
                            else{
                                messageToSend.push_front('1');
                                emit stages(1);
                            }

                        }
                        else if(moveSelected){
                            // Burada move komutu gönderilecek açılara göre
                            messageToSend.append(intToTwosComplement(shoulderServoAngle));
                            messageToSend.append(intToTwosComplement(elbowServoAngle));
                            messageToSend.push_front('3');
                            moveSelected = false;
                        }
                        else{
                            messageToSend = QByteArray::fromStdString("0Connection Established");
                        }

                        break;
                    }
                    case '1': //Image received command received
                    {
                        //Image için byte oku. Eğer byte kalmadıysa buradan  2 komutunu gönder (2 ile birlikte son byteları gönder). Eğer byte kaldıysa 1 komutunu gönder
                        // 2 ile gönderirken bütün değişkenleri temizle.
                        if(bytesWritten < imageByteArray.size()){
                            int remainingBytes = imageByteArray.size() - bytesWritten;
                            int bytesToWrite = qMin(chunkSize, remainingBytes);
                            messageToSend.append(imageByteArray.mid(bytesWritten, bytesToWrite));
                            bytesWritten += bytesToWrite;
                        }

                        if (bytesWritten >= imageByteArray.size()){
                            messageToSend.push_front('2');
                            emit stages(2);
                            bytesWritten = 0;
                            imageByteArray.clear();
                            drawSelected  = false;
                        }
                        else{
                            messageToSend.push_front('1');
                            //emit stages(1);
                        }

                        break;
                    }
                    case '2': //Preview of drawing received
                    {
                        jsonFile.append(receivedData);
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        break;
                    }
                    case '3': //Total line num received
                    {
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        QString dataString = QString::fromUtf8(jsonFile);

                        // Write the string data to a file
                        QString filePath = dir.path() + "/received.json";
                        QFile outputFile(filePath);
                        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                            QTextStream outputStream(&outputFile);
                            outputStream << dataString;
                            outputFile.close();
                            qDebug() << "Data written to file successfully";
                        } else {
                            qDebug() << "Unable to open file for writing";
                        }

                        QJsonParseError parseError;
                        QJsonDocument doc = QJsonDocument::fromJson(jsonFile, &parseError);
                        if (parseError.error != QJsonParseError::NoError) {
                            qDebug() << "Error parsing JSON file:" << parseError.errorString();
                            continue;
                        }

                        if (!doc.isArray()) {
                            qDebug() << "JSON document is not an array";
                            continue;
                        }

                        QJsonArray linesArray = doc.array();

                        emit allLinesReceived(linesArray);
                        emit stages(3);
                        totalLineNumberForAnImage = receivedData.toInt();

                        emit totalLineNumber(totalLineNumberForAnImage);
                        emit stats_number_of_lines_to_draw(totalLineNumberForAnImage);
                        break;
                    }
                    case '4': //Drawn line received
                    {
                        emit stages(4);
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        qDebug() << "Drawn line command";
                        qDebug() << receivedData;
                        QJsonParseError parseError;
                        QJsonDocument doc = QJsonDocument::fromJson(receivedData, &parseError);
                        if (parseError.error != QJsonParseError::NoError) {
                            qDebug() << "Error parsing JSON:" << parseError.errorString();
                            continue;
                        }

                        if (!doc.isArray()) {
                            qDebug() << "JSON document is not an array";
                            continue;
                        }

                        QJsonArray linesArray = doc.array();

                        // Emit signal to notify about received data
                        emit linesReceived(linesArray);
                        calculatedValue ++;
                        emit loadingProgress(calculatedValue);
                        emit stats_number_of_drawn_line(calculatedValue);
                        if(calculatedValue == totalLineNumberForAnImage){
                            emit drawingStatus(false);
                            jsonFile.clear();
                            calculatedValue = 0;
                        }
                        break;
                    }
                    case '5': //Servo angles received
                    {
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        QByteArray firstPart = receivedData.mid(0, 16); // Extract first 16 bytes
                        QByteArray secondPart = receivedData.mid(16, 16); // Extract next 16 bytes
                        QByteArray thirdPart = receivedData.mid(32, 16); // Extract final 16 bytes
                        emit servoAngles(convertTwosComplement(firstPart.toStdString()), convertTwosComplement(secondPart.toStdString()),convertTwosComplement(thirdPart.toStdString()));
                        break;
                    }
                    case '6': //Sensor data received
                    {
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        //int sensorData = convertTwosComplement(receivedData.toStdString());
                        //double sensorData = convertTwosComplement(receivedData.toStdString());
                        double sensorData = receivedData.toDouble();
                        emit sensorValues(sensorData);
                        break;
                    }
                    default:
                        qDebug() << "No message received";
                        break;
                    }
                }
            }
            //Burada ekstradan bir messageToSend tutabilirsin. Onun içerisinde göndermek istediğin mesajlar olur (örnek olarak açı ayarı. Bu olduğunda o komut gitmeli. 0 durumunda bu komut ile alakalı degerleri
            // karsı tarafa gönderebilirsin. Karşı taraftan da işi bitince sana sıfır komutu gelir. "Stop Drawing" komutu ile karşı tarafta resim çizen threadleri sleep yapabilirsin.
            //Cancel drawing komutu için ise karşı tarafta çalışan server2() threadini cancella.
            if (!messageToSend.isEmpty() && !receivedData.isEmpty()) {
                tcpSocket->write(messageToSend); // Sending the message to the server
                //qDebug()  << messageToSend << " is sent!";
            }
            QThread::msleep(100);
        }

        tcpSocket->disconnectFromHost();
        if(tcpSocket->waitForDisconnected()){
            connected = false;
        }
        tcpSocket->close();
        delete tcpSocket;
    }
}

bool ServerListenerThread::isConnected(){
    return connected;
}

void ServerListenerThread::move(bool isMoveButtonClicked, int shoulderAngle, int elbowAngle){
    qDebug() << "Move button clicked!";
    moveSelected = isMoveButtonClicked;
    shoulderServoAngle = shoulderAngle;
    elbowServoAngle = elbowAngle;
}

void ServerListenerThread::draw(bool isDrawButtonClicked){
    qDebug() << "Draw button clicked!";
    drawSelected = isDrawButtonClicked;
}

void ServerListenerThread::socketDisconnected() {
    //QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (tcpSocket != nullptr && tcpSocket) {
        emit drawingStatus(false);
        emit loadingProgress(-1);
        qDebug() << "Socket disconnected from the server.";
        //tcpSocket->disconnectFromHost();
        //tcpSocket->deleteLater(); // Delete the socket properly
    }
}

void ServerListenerThread::updateServerAddress(const QString& ipAddress, int port) {
    mIpAddress = ipAddress;
    mPort = port;
    run();
}

