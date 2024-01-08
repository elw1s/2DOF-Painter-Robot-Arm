#ifndef SERVERLISTENERTHREAD_H
#define SERVERLISTENERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QBuffer>

#include "RobotProjectionWidget.h" // Include the header for RobotProjectionWidget

class ServerListenerThread : public QThread {
    Q_OBJECT

signals:
    void linesReceived(const QJsonArray& lines);
    void allLinesReceived(const QJsonArray& lines);
    void loadingProgress(const int calculatedValue);
    void drawingStatus(const bool status);
    void totalLineNumber(const int num);
    void servoAngles(const int angle1, const int angle2, const int angle3);
    void sensorValues(const double sensorVal);
    void stats_received_packet_num(const int received_packet_num);
    void stats_last_packet_size(const double last_packet_size);
    void stats_number_of_drawn_line(const int number_of_drawn_line);
    void stats_number_of_lines_to_draw(const int number_of_lines_to_draw);
    void stages(const int value);
public:
    void run() override;
    ServerListenerThread(const QString& ipAddress, int port, QObject *parent = nullptr);
    void socketDisconnected();
    void move(bool isMoveButtonClicked, int shoulderAngle, int elbowAngle);
    void draw(bool isDrawButtonClicked);
    bool isConnected();
    void applyBorder(const QString filePath);
private:
    QString mIpAddress;
    int mPort;
    bool drawSelected;
    bool moveSelected;
    QTcpSocket* tcpSocket;
    int shoulderServoAngle;
    int elbowServoAngle;
    int liftingServoAngle;
    bool connected;
    int received_packet_num;

public slots:
    void updateServerAddress(const QString& ipAddress, int port);
};

#endif // SERVERLISTENERTHREAD_H
