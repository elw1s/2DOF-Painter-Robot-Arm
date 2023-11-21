#include "ServerListenerThread.h"

ServerListenerThread::ServerListenerThread(QObject *parent)
    : QThread(parent) {
    // ... constructor implementation ...
}

void ServerListenerThread::run() {
    qDebug() << "Run works";

    while (!isInterruptionRequested()) {
        QTcpSocket* tcpSocket = new QTcpSocket();

        // Attempt to connect to the TCP server
        tcpSocket->connectToHost("127.0.0.1", 12346); // Replace with your server's IP and port
        if (!tcpSocket->waitForConnected(3000)) { // 3-second timeout for connection attempt
            qDebug() << "Error: Cannot connect to the server. Retrying in 3 seconds...";
            tcpSocket->close();
            delete tcpSocket;
            QThread::sleep(3); // Sleep for 3 seconds before next attempt
            continue;
        }

        qDebug() << "Connected to the server.";

        while (!isInterruptionRequested()) {
            QByteArray receivedData;
            if (tcpSocket->waitForReadyRead()) {
                receivedData = tcpSocket->readAll();

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
                qDebug() << "Before emit";
                emit linesReceived(linesArray);
                qDebug() << "linesArray emitted";
            }
        }

        tcpSocket->close();
        delete tcpSocket;
    }
}


