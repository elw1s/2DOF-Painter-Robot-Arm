#include "StatsApp.h"

StatsApp::StatsApp(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);
    QHBoxLayout *rightLayout = new QHBoxLayout(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout(this);
    receivedPacketWithinOneSecond = 0;
    receivedPacketsLabel = new QLabel("Number of received packets: 0");
    receivedLinesLabel = new QLabel("Number of received lines: 0");
    totalLinesLabel = new QLabel("Number of total lines: 0");
    lastPacketSizeLabel = new QLabel("Last packet size: 0 KB");
    receivedPacketsLabel->setFont(QFont(receivedPacketsLabel->font().family(), 18));
    receivedLinesLabel->setFont(QFont(receivedLinesLabel->font().family(), 18));
    totalLinesLabel->setFont(QFont(totalLinesLabel->font().family(), 18));
    lastPacketSizeLabel->setFont(QFont(lastPacketSizeLabel->font().family(), 18));
    leftLayout->setSpacing(0);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->addWidget(receivedPacketsLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    leftLayout->addWidget(receivedLinesLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    leftLayout->addWidget(totalLinesLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    leftLayout->addWidget(lastPacketSizeLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

    topLayout->addLayout(leftLayout);
    mainLayout->addLayout(topLayout);

    plotLayout = new QHBoxLayout;
    customPlot = new QCustomPlot(this);
    customPlot->setInteraction(QCP::iRangeDrag, true); // Enable dragging the plot range
    customPlot->setInteraction(QCP::iRangeZoom, true); // Enable zooming using the mouse wheel
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    customPlot->setObjectName(QStringLiteral("customPlot"));
    customPlot->setBackground(QBrush(QColor("#4F4F4F"))); // Set plot background color
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Received Packet Number");
    customPlot->xAxis->setTickLabelColor(QColor("#33C2FF")); // Set tick and subtick colors
    customPlot->yAxis->setTickLabelColor(QColor("#33C2FF"));
    customPlot->xAxis->setTickPen(QColor("white"));
    customPlot->yAxis->setTickPen(QColor("white"));
    customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTickerFixed>(new QCPAxisTickerFixed));

    plotUpdateTimer = new QTimer(this);
    plotUpdateTimer->setInterval(1000);
    connect(plotUpdateTimer, &QTimer::timeout, this, &StatsApp::updatePacketFrequencyPlot);
    plotUpdateTimer->start();

    plotResetTimer = new QTimer(this);
    plotResetTimer->setInterval(120000);
    connect(plotResetTimer, &QTimer::timeout, this, &StatsApp::resetPlot);
    plotResetTimer->start();



    plotLayout->addWidget(customPlot);
    mainLayout->addLayout(plotLayout);
    setLayout(mainLayout);
}

// Slot to update number of received packets label
void StatsApp::updateReceivedPackets(int numPackets) {
    receivedPacketsLabel->setText("Number of received packets: " + QString::number(numPackets));
    receivedPacketWithinOneSecond++;
}

// Slot to update number of received lines label
void StatsApp::updateNumberOfDrawnLine(int numLines) {
    receivedLinesLabel->setText("Number of drawn lines: " + QString::number(numLines));
}

// Slot to update mean of packet sizes label
void StatsApp::updateNumberOfLinesToDraw(int numLines) {
    totalLinesLabel->setText("Number of total lines: " + QString::number(numLines));
}

void StatsApp::updateLastPacketSize(double size) {
    double sizeInKB = size / 1024.0;
    lastPacketSizeLabel->setText("Last packet size: " + QString::number(sizeInKB, 'f', 3) + " KB");
}

void StatsApp::resetPlot(){
    timeDataPlot.clear();
    receivedSizePlot.clear();
}

void StatsApp::updatePacketFrequencyPlot() {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int packetsWithinOneSecond = receivedPacketWithinOneSecond;
    receivedPacketWithinOneSecond = 0;
    timeDataPlot.append(currentDateTime.toMSecsSinceEpoch() / 1000.0);
    receivedSizePlot.append(packetsWithinOneSecond);

    customPlot->clearPlottables();
    QCPGraph *scatter = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    scatter->setLineStyle(QCPGraph::lsLine);
    scatter->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    scatter->setPen(QPen(QColor("#19749B")));
    scatter->setBrush(QColor("#33C2FF"));
    scatter->setData(timeDataPlot, receivedSizePlot);

    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setTicker(dateTimeTicker);

    customPlot->xAxis->setRange(timeDataPlot.first(), timeDataPlot.last());

    double maxY = *std::max_element(receivedSizePlot.constBegin(), receivedSizePlot.constEnd());
    double minY = *std::min_element(receivedSizePlot.constBegin(), receivedSizePlot.constEnd());
    double margin = 0.1 * (maxY - minY);
    customPlot->yAxis->setRange(minY - margin, maxY + margin);

    customPlot->replot();
}

