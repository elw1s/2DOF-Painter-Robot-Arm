#ifndef GAZEBO_H
#define GAZEBO_H

#include <QString>
#include <QProcess>
#include <QThread>
#include <QTcpSocket>

class Gazebo : public QThread {

private:
    struct PositionCommand {
        double joint1;
        double joint2;
        double joint3;
    };
    QTcpSocket* tcpSocket;
    const QString projectPath;

public:
    Gazebo();

    // Function to run Gazebo project through command line
    //void runGazeboProject(const QString& projectPath);

protected:
    void run() override;
};

#endif // GAZEBO_H
