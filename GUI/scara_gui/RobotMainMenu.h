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
    QList<QString> getColors();
    void updateSensorGraph(double sensorValue);
    void updateServoAngleGraph(int firstAngle, int secondAngle, int thirdAngle);
    void onUpdateTimerServo();
    void onUpdateTimerSensor();
    void drawButtonClicked();
    void moveButtonClicked();
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
    QList<QString> colors;
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
    QPushButton* setButton;
    QPushButton* defaultButton;
    QSlider *sliderRed;
    QSlider *sliderBlue;
public slots:
    void setServerInfo(const QString& ip, int port);
    void setColors(const QList<QString>& colorArr);
    void disconnectFromServer();
    void initializeServerListener();
    void showLoadingBar(int value); // Show the loading bar with a specific value
    void setTotalLineNumber(int totalLineNumber);
    void defaultButtonClicked();
    void setButtonClicked();
    void robotDrawingSignal(const bool status);
    void stats_received_packet_num_slot(const int received_packet_num);
    void stats_last_packet_size_slot(const double last_packet_size);
    void stats_number_of_drawn_line_slot(const int number_of_drawn_line);
    void stats_number_of_lines_to_draw_slot(const int number_of_lines_to_draw);
    void stagesTextLabel(int value);

signals:
    void drawingStatus(const bool status);
    void stats_received_packet_num(const int received_packet_num);
    void stats_last_packet_size(const double last_packet_size);
    void stats_number_of_drawn_line(const int number_of_drawn_line);
    void stats_number_of_lines_to_draw(const int number_of_lines_to_draw);
    void sendServoAngles(const double angle_1, const double angle_2, const double angle_3);
};

#endif // ROBOTMAINMENU_H
