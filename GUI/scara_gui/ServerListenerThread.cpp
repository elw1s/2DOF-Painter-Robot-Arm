#include "ServerListenerThread.h"

ServerListenerThread::ServerListenerThread(const QString& ipAddress, int port, QObject *parent)
    : QThread(parent), mIpAddress(ipAddress), mPort(port) {
    // ... constructor implementation ...
    emit loadingProgress(-1);
    drawSelected = false;
    connected = false;
    moveSelected = false;
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

void ServerListenerThread::run() {
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
        QByteArray jsonFile;
        QByteArray imageByteArray;
        int chunkSize = 4096 - 1; // 4KB - 1
        int bytesWritten = 0;
        while (!isInterruptionRequested()) {
            QByteArray receivedData;
            if (tcpSocket->waitForReadyRead(5000)) {
                receivedData = tcpSocket->readAll();
                qDebug() <<"Received:" << receivedData;

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
                            // DOSYA PATH DEGİSTİR!!!
                            QFile file("/home/arda/Desktop/CSE396/BrachioGraph/images/cat.jpeg");
                            if (!file.open(QIODevice::ReadOnly)) {
                                qDebug() << "Failed to open the image file.";
                                // Handle error or return an empty byte array
                                //return QByteArray();
                                continue;
                            }

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
                                bytesWritten = 0;
                                imageByteArray.clear();
                                drawSelected  = false;
                            }
                            else{
                                messageToSend.push_front('1');
                            }

                        }
                        else if(moveSelected){
                            // Burada move komutu gönderilecek açılara göre
                            messageToSend = QByteArray::fromStdString("0Connection Established");
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
                            bytesWritten = 0;
                            imageByteArray.clear();
                            drawSelected  = false;
                        }
                        else{
                            messageToSend.push_front('1');
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
                        qDebug() << "Number of lines command";
                        qDebug()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         << receivedData.toInt() << "";

                        qDebug() << ".......................................";
                        qDebug() << jsonFile;
                        qDebug() << ".......................................";

                        QString dataString = QString::fromUtf8(jsonFile);

                        // Write the string data to a file
                        QFile outputFile("/home/arda/Desktop/CSE396/simulate_embedded/tmp/received.json");
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
                        emit totalLineNumber(receivedData.toInt());
                        break;
                    }
                    case '4': //Drawn line received
                    {
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        qDebug() << "Drawn line command";
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
                        emit loadingProgress(calculatedValue);
                        calculatedValue ++;
                        break;
                    }
                    case '5': //Servo angles received
                    {
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        qDebug() << "Servo Angles command";
                        QByteArray firstPart = receivedData.mid(0, 16); // Extract first 16 bytes
                        QByteArray secondPart = receivedData.mid(16, 16); // Extract next 16 bytes
                        QByteArray thirdPart = receivedData.mid(32, 16); // Extract final 16 bytes
                        qDebug() << "First Servo Angle:" << firstPart.toStdString();
                        qDebug() << "Second Servo Angle:" << secondPart.toStdString();
                        qDebug() << "Third Servo Angle:" << thirdPart.toStdString();
                        emit servoAngles(convertTwosComplement(firstPart.toStdString()), convertTwosComplement(secondPart.toStdString()),convertTwosComplement(thirdPart.toStdString()));
                        break;
                    }
                    case '6': //Sensor data received
                    {
                        messageToSend = QByteArray::fromStdString("6Connection Established");
                        qDebug() << "Sensor data command";
                        int sensorData = convertTwosComplement(receivedData.toStdString());
                        qDebug() << "Sensor Data:" << sensorData;
                        emit sensorValues(convertTwosComplement(receivedData.toStdString()));
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
                qDebug()  << messageToSend << " is sent!";
            }
            //QThread::sleep(1);
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

void ServerListenerThread::move(bool isMoveButtonClicked, int shoulderAngle, int elbowAngle, int liftingAngle){
    qDebug() << "Move button clicked!";
    moveSelected = isMoveButtonClicked;
    shoulderServoAngle = shoulderAngle;
    elbowServoAngle = elbowAngle;
    liftingServoAngle = liftingAngle;
}

void ServerListenerThread::draw(bool isDrawButtonClicked){
    qDebug() << "Draw button clicked!";
    drawSelected = isDrawButtonClicked;
}

void ServerListenerThread::socketDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        qDebug() << "Socket disconnected from the server.";
        socket->deleteLater(); // Delete the socket properly
    }
}

void ServerListenerThread::updateServerAddress(const QString& ipAddress, int port) {
    mIpAddress = ipAddress;
    mPort = port;
    run();
}

