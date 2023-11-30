#ifndef ROBOTMAINMENU_H
#define ROBOTMAINMENU_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QScrollArea>
#include <QMovie>
#include <QProgressBar>
#include <QTime>
#include <QVector>

#include "RobotProjectionWidget.h"
#include "ServerListenerThread.h"
#include "ImagePathsConfig.h"
#include "qcustomplot.h"

class RobotMainMenu : public QWidget {
    Q_OBJECT

public:
    RobotMainMenu(QWidget *parent = nullptr);
    QString getIP();
    int getPort();
    void updateSensorGraph(double sensorValue);
    void updateServoAngleGraph(int firstAngle, int secondAngle, int thirdAngle);
    void onUpdateTimerServo();
    void onUpdateTimerSensor();
private:
    QLabel *label;
    RobotProjectionWidget *projectionWidget;
    ServerListenerThread* serverListenerThread;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *bottomButton;
    QVBoxLayout *mainLayout;
    QHBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QString ipAddress;
    int port;
    int totalLine;
    QMovie *loadingMovie;
    QProgressBar *loadingProgressBar;
    QLabel *loadingLabel;
    QHBoxLayout *loadingLabelLayout;
    QLabel *textLabel;
    QCustomPlot *sensorPlot;
    QCustomPlot *waveformPlot;
    QLabel *redLabel;
    QLabel *greenLabel;
    QLabel *blueLabel;
    QVector<double> timeDataSensor;
    QVector<double> sensorData;
    QVector<double> timeDataServo; // Vector for X-axis (time)
    QVector<double> firstAngleData; // Vector for first servo angle data (green)
    QVector<double> secondAngleData; // Vector for second servo angle data (red)
    QVector<double> thirdAngleData; // Vector for third servo angle data (blue)
    QTimer *updateTimerServo;
    QTimer *updateTimerSensor;
public slots:
    void setServerInfo(const QString& ip, int port);
    void disconnectFromServer();
    void initializeServerListener();
    void showLoadingBar(int value); // Show the loading bar with a specific value
    void setTotalLineNumber(int totalLineNumber);
};

#endif // ROBOTMAINMENU_H
