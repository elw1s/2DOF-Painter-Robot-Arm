#include "ServerListenerThread.h"

ServerListenerThread::ServerListenerThread(const QString& ipAddress, int port, QObject *parent)
    : QThread(parent), mIpAddress(ipAddress), mPort(port) {
    // ... constructor implementation ...
    emit loadingProgress(-1);
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
        emit loadingProgress(0);
        int calculatedValue = 0;
        while (!isInterruptionRequested()) {
            QByteArray receivedData;
            if (tcpSocket->waitForReadyRead()) {
                receivedData = tcpSocket->readAll();
                qDebug() <<"Received:" << receivedData;

                if (!receivedData.isEmpty()) {
                    char command = receivedData[0];
                    receivedData = receivedData.remove(0, 1); // Removes the first element

                    switch(command){
                    case '0':
                        qDebug() << "Number of lines command";
                        qDebug() << receivedData.toInt() << "";
                        emit totalLineNumber(receivedData.toInt());
                        break;
                    case '1': {
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
                    case '2': {
                        qDebug() << "Sensor data command";
                        int sensorData = receivedData.toInt();
                        qDebug() << "Sensor Data:" << sensorData;
                        emit sensorValues(receivedData.toDouble());
                        break;
                    }
                    case '3': {
                        qDebug() << "Servo Angles command";
                        QByteArray firstPart = receivedData.mid(0, 16); // Extract first 16 bytes
                        QByteArray secondPart = receivedData.mid(16, 16); // Extract next 16 bytes
                        QByteArray thirdPart = receivedData.mid(32, 16); // Extract final 16 bytes
                        qDebug() << "First Servo Angle:" << firstPart.toStdString();
                        qDebug() << "Second Servo Angle:" << secondPart.toStdString();
                        qDebug() << "Third Servo Angle:" << thirdPart.toStdString();
                        emit servoAngles(firstPart.toInt(), secondPart.toInt(),thirdPart.toInt());
                        break;
                    }
                    case '4':
                        qDebug() << "Idle command";
                        break;
                    default:
                        qDebug() << "Unknown command";
                        break;
                    }
                }
            }
        }

        tcpSocket->disconnectFromHost();
        tcpSocket->waitForDisconnected();
        tcpSocket->close();
        delete tcpSocket;
    }
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

