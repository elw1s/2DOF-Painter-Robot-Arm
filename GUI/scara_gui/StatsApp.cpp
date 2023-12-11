#include "StatsApp.h"

StatsApp::StatsApp(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);

    // First horizontal layout
    firstHBoxLayout = new QHBoxLayout;
    receivedPacketsLabel = new QLabel("Number of received packets: ");
    receivedLinesLabel = new QLabel("Number of received lines: ");
    firstHBoxLayout->addWidget(receivedPacketsLabel);
    firstHBoxLayout->addWidget(receivedLinesLabel);
    firstHBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->addLayout(firstHBoxLayout);

    // Second horizontal layout
    secondHBoxLayout = new QHBoxLayout;
    meanPacketSizesLabel = new QLabel("Mean of packet sizes: ");
    lastPacketSizeLabel = new QLabel("Last packet size: ");
    secondHBoxLayout->addWidget(meanPacketSizesLabel);
    secondHBoxLayout->addWidget(lastPacketSizeLabel);
    secondHBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->addLayout(secondHBoxLayout);

    setLayout(mainLayout);
}
