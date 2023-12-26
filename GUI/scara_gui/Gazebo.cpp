#include "Gazebo.h"

Gazebo::Gazebo() {
    //runGazeboProject("");
    //projectPath = "";
}

/*void Gazebo::runGazeboProject(const QString& projectPath) {
    QProcess gazeboProcess;
    QStringList arguments;

    // Assuming 'gazebo' is the command to run Gazebo with a given project path
    arguments << projectPath;

    // Run the Gazebo project through the command line
    gazeboProcess.start("gazebo", arguments);
    gazeboProcess.waitForFinished(); // Wait for the process to finish
}*/

void Gazebo::run() {
    QProcess gazeboProcess;
    QStringList arguments;

    arguments << projectPath;

    gazeboProcess.start("gazebo", arguments);
    //gazeboProcess.waitForFinished(); // Wait for the process to finish
    while (!isInterruptionRequested()) {
        tcpSocket = new QTcpSocket();
        tcpSocket->connectToHost("127.0.0.1",8081);
        if (!tcpSocket->waitForConnected(3000)) { // 3-second timeout for connection attempt
            qDebug() << "Error: Cannot connect to the server. Retrying in 3 seconds...";
            tcpSocket->close();
            delete tcpSocket;
            QThread::sleep(3); // Sleep for 3 seconds before next attempt
            continue;
        }
        qDebug() << "Connected to the gazebo.";
        QByteArray messageToSend;

        while (!isInterruptionRequested()){

            if (!messageToSend.isEmpty() && tcpSocket->state() == QAbstractSocket::ConnectedState) {
                tcpSocket->write(messageToSend); // Sending the message to the server
                qDebug()  << messageToSend << " is sent! (FIRST)";
                qDebug() << "Message sending...";
                //tcpSocket->waitForBytesWritten(); // Wait for the message to be sent (optional)
                qDebug() << "Message sent...";
            }
        }

    }




}
