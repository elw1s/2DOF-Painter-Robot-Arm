#ifndef SERVERLISTENERTHREAD_H
#define SERVERLISTENERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QBuffer>

#include "RobotProjectionWidget.h" // Include the header for RobotProjectionWidget

class ServerListenerThread : public QThread {
    Q_OBJECT

signals:
    void linesReceived(const QJsonArray& lines);
    void allLinesReceived(const QJsonArray& lines);
    void loadingProgress(const int calculatedValue);
    void totalLineNumber(const int num);
    void servoAngles(const int angle1, const int angle2, const int angle3);
    void sensorValues(const double sensorVal);
public:
    void run() override;
    ServerListenerThread(const QString& ipAddress, int port, QObject *parent = nullptr);
    void socketDisconnected();
private:
    QString mIpAddress;
    int mPort;
    bool draw;
    QTcpSocket* tcpSocket;

public slots:
    void updateServerAddress(const QString& ipAddress, int port);
};

#endif // SERVERLISTENERTHREAD_H
