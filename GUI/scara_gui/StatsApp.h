#ifndef STATSAPP_H
#define STATSAPP_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "qcustomplot.h"

class StatsApp : public QWidget {
    Q_OBJECT

public:
    StatsApp(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *firstHBoxLayout;
    QHBoxLayout *secondHBoxLayout;
    QHBoxLayout *plotLayout;
    QCustomPlot *customPlot;
    QLabel *receivedPacketsLabel;
    QLabel *receivedLinesLabel;
    QLabel *totalLinesLabel;
    QLabel *lastPacketSizeLabel;
    double receivedPacketSizes;
    QVector<double> timeDataPlot;
    QVector<double> receivedSizePlot;

public slots:
    void updateReceivedPackets(int numPackets);
    void updateNumberOfDrawnLine(int numLines);
    void updateNumberOfLinesToDraw(int numLines);
    void updateLastPacketSize(double size);
};

#endif // STATSAPP_H
