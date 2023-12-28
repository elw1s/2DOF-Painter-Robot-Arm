#ifndef GAZEBO_H
#define GAZEBO_H

#include <QString>
#include <QProcess>
#include <QThread>
#include <QTcpSocket>
#include <QtEndian>

class Gazebo : public QThread {
    Q_OBJECT
private:
    struct PositionCommand {
        double joint1;
        double joint2;
        double joint3;
    }servoAngles;
    QTcpSocket* tcpSocket;
    QProcess gazeboProcess;
    const QString projectPath;
    bool shouldRun;
public:
    Gazebo();
    void startExecution();
    void stopExecution();
    // Function to run Gazebo project through command line
    //void runGazeboProject(const QString& projectPath);

public slots:
    void setServoAngles(double angle_1, double angle_2, double angle_3);
    void onGazeboProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    void run() override;
};

#endif // GAZEBO_H
