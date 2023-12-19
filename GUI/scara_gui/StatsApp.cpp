#include "StatsApp.h"

StatsApp::StatsApp(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);

    // First horizontal layout
    firstHBoxLayout = new QHBoxLayout;
    receivedPacketsLabel = new QLabel("Number of received packets: 0");
    receivedLinesLabel = new QLabel("Number of received lines: 0");
    firstHBoxLayout->addWidget(receivedPacketsLabel);
    firstHBoxLayout->addWidget(receivedLinesLabel);
    firstHBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->addLayout(firstHBoxLayout);

    // Second horizontal layout
    secondHBoxLayout = new QHBoxLayout;
    totalLinesLabel = new QLabel("Number of total lines: 0");
    lastPacketSizeLabel = new QLabel("Last packet size: 0 KB");
    secondHBoxLayout->addWidget(totalLinesLabel);
    secondHBoxLayout->addWidget(lastPacketSizeLabel);
    secondHBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->addLayout(secondHBoxLayout);

    plotLayout = new QHBoxLayout;
    customPlot = new QCustomPlot(this);
    customPlot->setObjectName(QStringLiteral("customPlot"));
    customPlot->setBackground(QBrush(QColor("#4F4F4F"))); // Set plot background color
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Received Packet Sizes");
    customPlot->xAxis->setTickLabelColor(QColor("#33C2FF")); // Set tick and subtick colors
    customPlot->yAxis->setTickLabelColor(QColor("#33C2FF"));
    customPlot->xAxis->setTickPen(QColor("white"));
    customPlot->yAxis->setTickPen(QColor("white"));
    plotLayout->addWidget(customPlot);
    mainLayout->addLayout(plotLayout);
    setLayout(mainLayout);
}

// Slot to update number of received packets label
void StatsApp::updateReceivedPackets(int numPackets) {
    receivedPacketsLabel->setText("Number of received packets: " + QString::number(numPackets));
}

// Slot to update number of received lines label
void StatsApp::updateNumberOfDrawnLine(int numLines) {
    receivedLinesLabel->setText("Number of drawn lines: " + QString::number(numLines));
}

// Slot to update mean of packet sizes label
void StatsApp::updateNumberOfLinesToDraw(int numLines) {
    totalLinesLabel->setText("Number of total lines: " + QString::number(numLines));
}

// Slot to update last packet size label
void StatsApp::updateLastPacketSize(double size) {

//    QDateTime currentDateTime = QDateTime::currentDateTime(); // Get current date and time

//    if (receivedSizePlot.size() >= 10) {
//        receivedSizePlot.removeFirst();
//        timeDataPlot.removeFirst();
//    }

//    timeDataPlot.append(currentDateTime.toMSecsSinceEpoch() / 1000.0); // Convert QDateTime to seconds since epoch
//    receivedSizePlot.append(size / 1024.0);

//    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
//    dateTimeTicker->setDateTimeFormat("hh:mm:ss"); // Set tick label format
//    dateTimeTicker->setTickCount(3); // Show only four ticks
//    dateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
//    customPlot->xAxis->setTicker(dateTimeTicker); // Set ticker

//    QCPBars* bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
//    bars->setWidth(9/(double)timeDataPlot.size());
//    bars->setPen(Qt::NoPen);
//    bars->setBrush(QColor("#33C2FF"));
//    bars->setData(timeDataPlot, receivedSizePlot);

//    customPlot->xAxis->setRange(-0.5, qMax(9.5, timeDataPlot.size() - 0.5));
//    double maxY = *std::max_element(receivedSizePlot.constBegin(), receivedSizePlot.constEnd());
//    customPlot->yAxis->setRange(0, maxY); // Set minimum to 0 and maximum to maxY
//    customPlot->replot();

    double sizeInKB = size / 1024.0;
    lastPacketSizeLabel->setText("Last packet size: " + QString::number(sizeInKB, 'f', 2) + " KB");
}
