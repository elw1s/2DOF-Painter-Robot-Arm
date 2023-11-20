#include "RobotMainMenu.h"

RobotMainMenu::RobotMainMenu(QWidget *parent) : QWidget(parent) {
    label = new QLabel("Projection of Robot");
    projectionWidget = new RobotProjectionWidget(this);
    button1 = new QPushButton("Button 1");
    button2 = new QPushButton("Button 2");
    bottomButton = new QPushButton("Bottom Button");

    projectionWidget->setStyleSheet("background-color: #D9D9D9; border: 5px solid red;");
    projectionWidget->setPointsData("/home/arda/Desktop/CSE396/BrachioGraph/images/cat.json");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(label);
    leftLayout->addWidget(projectionWidget);

    QVBoxLayout *rightTopLayout = new QVBoxLayout;
    rightTopLayout->addWidget(button1);
    rightTopLayout->addWidget(button2);

    QVBoxLayout *rightMiddleLayout = new QVBoxLayout;
    // Add 2D plot widgets here

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(rightTopLayout);
    rightLayout->addLayout(rightMiddleLayout);
    rightLayout->addWidget(bottomButton);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
}
