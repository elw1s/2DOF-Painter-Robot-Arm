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

#include "RobotProjectionWidget.h"
#include "ServerListenerThread.h"

class RobotMainMenu : public QWidget {
    Q_OBJECT

public:
    RobotMainMenu(QWidget *parent = nullptr);

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
    void initializeServerListener();
};

#endif // ROBOTMAINMENU_H
