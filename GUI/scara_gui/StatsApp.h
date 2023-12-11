#ifndef STATSAPP_H
#define STATSAPP_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>


class StatsApp : public QWidget {
    Q_OBJECT

public:
    StatsApp(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *firstHBoxLayout;
    QHBoxLayout *secondHBoxLayout;

    QLabel *receivedPacketsLabel;
    QLabel *receivedLinesLabel;
    QLabel *meanPacketSizesLabel;
    QLabel *lastPacketSizeLabel;
};

#endif // STATSAPP_H
