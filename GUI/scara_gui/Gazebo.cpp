#include "Gazebo.h"

Gazebo::Gazebo(){
    shouldRun = false;
    servoAngles.joint1 = -90;
    servoAngles.joint2 = 90;
    servoAngles.joint3 = 1200;
}

void Gazebo::run() {

    if (!shouldRun) {
        return; // Exit if shouldRun flag is false
    }

    QStringList arguments;

    arguments << projectPath;

    arguments.append("172.20.10.13");
    arguments.append("8081");

    //connect(&gazeboProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
    //         this, SLOT(onGazeboProcessFinished(int,QProcess::ExitStatus)));

    //gazeboProcess.start("gazebo", arguments);
    //gazeboProcess.start("cd brachiograph_ws/docker/ && sudo ./", arguments);

    // if (!gazeboProcess.waitForStarted()) {
    //     qDebug() << "Error: Unable to start the Gazebo process.";
    //     stopExecution();
    //     return;
    // }

    qDebug() << "Gazebo process started.";

    while (!isInterruptionRequested()) {
        tcpSocket = new QTcpSocket();
        tcpSocket->connectToHost("127.0.0.1",3800);
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
            if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
                messageToSend.append(reinterpret_cast<const char*>(&servoAngles), sizeof(servoAngles));
                tcpSocket->write(messageToSend); // Sending the serialized struct
                tcpSocket->waitForBytesWritten(); // Wait for the message to be sent
                messageToSend.clear();
            }
            QThread::msleep(1000);
        }

        // Disconnection occurred
        qDebug() << "Disconnected from the gazebo.";
        tcpSocket->close();
        delete tcpSocket;
        stopExecution();
    }
}

void Gazebo::startExecution() {
    shouldRun = true;
    qDebug() << "Gazebo is started...";
    start();
}

void Gazebo::stopExecution() {
    shouldRun = false;

    if (gazeboProcess.state() == QProcess::Running) {
        gazeboProcess.terminate(); // Terminate the Gazebo process if it's running
        gazeboProcess.waitForFinished(); // Wait for the process to finish termination
    }

    requestInterruption();

    qDebug() << "Gazebo is terminated...";
}

void Gazebo::onGazeboProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    // Handle the closure of gazeboProcess here
    qDebug() << "Gazebo process has finished with exit code:" << exitCode;

    // Close and delete the tcpSocket when gazeboProcess finishes
    if (tcpSocket && tcpSocket->state() != QAbstractSocket::UnconnectedState) {
        requestInterruption();
    }
}

void Gazebo::setServoAngles(double angle_1, double angle_2, double angle_3){
    servoAngles.joint1 = angle_1;
    servoAngles.joint2 = angle_2;
    servoAngles.joint3 = angle_3;
}
