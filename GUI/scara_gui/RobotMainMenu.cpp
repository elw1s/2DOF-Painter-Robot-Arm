#include "RobotMainMenu.h"

RobotMainMenu::RobotMainMenu(QWidget *parent) : QWidget(parent) {
    label = new QLabel("Preview of drawing");
    projectionWidget = new RobotProjectionWidget(this);
    button1 = new QPushButton("Button 1");
    button2 = new QPushButton("Button 2");
    bottomButton = new QPushButton("View 3D Virtualization");
    bottomButton->setObjectName("bottomButton");
    bottomButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bottomButton->setFixedWidth(250);
    bottomButton->setFixedHeight(50); // Set the fixed height
    bottomButton->setStyleSheet("QPushButton#bottomButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 75px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#bottomButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");


    projectionWidget->setStyleSheet("background-color: #D9D9D9; border: 5px solid red;");

    /**/
    //QFile file("/home/arda/Desktop/CSE396/BrachioGraph/images/cat.json"); // Replace with your JSON file path

    //projectionWidget->setPointsData("/home/arda/Desktop/CSE396/BrachioGraph/images/cat.json");
    /*if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file.";
        //return -1;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray()) {
        qDebug() << "Invalid JSON format.";
        //return -1;
    }

    QJsonArray jsonArray = jsonDoc.array();
    projectionWidget->loadLinesFromJson(jsonArray);*/
    //projectionWidget->show();
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(projectionWidget);
    /**/
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(label,10,Qt::AlignCenter);
    //leftLayout->addWidget(projectionWidget,90);
    leftLayout->addWidget(scrollArea,90);
    QVBoxLayout *rightTopLayout = new QVBoxLayout;
    rightTopLayout->addWidget(button1);
    rightTopLayout->addWidget(button2);

    QVBoxLayout *rightMiddleLayout = new QVBoxLayout;
    // Add 2D plot widgets here

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(rightTopLayout);
    rightLayout->addLayout(rightMiddleLayout);
    rightLayout->addStretch(); // Add a stretch before the bottom button
    rightLayout->addWidget(bottomButton);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    initializeServerListener();
}

void RobotMainMenu::initializeServerListener() {
    serverListenerThread = new ServerListenerThread(this);
    connect(serverListenerThread, &ServerListenerThread::linesReceived,
            projectionWidget, &RobotProjectionWidget::loadLinesFromJson);
    serverListenerThread->start();
}
