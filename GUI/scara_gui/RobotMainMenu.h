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
    QVector<double> timeData;
    QVector<double> sensorData;

public slots:
    void setServerInfo(const QString& ip, int port);
    void disconnectFromServer();
    void initializeServerListener();
    void showLoadingBar(int value); // Show the loading bar with a specific value
    void setTotalLineNumber(int totalLineNumber);
};

#endif // ROBOTMAINMENU_H
