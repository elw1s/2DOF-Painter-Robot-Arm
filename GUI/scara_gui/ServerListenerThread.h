#ifndef SERVERLISTENERTHREAD_H
#define SERVERLISTENERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include "RobotProjectionWidget.h" // Include the header for RobotProjectionWidget

class ServerListenerThread : public QThread {
    Q_OBJECT

signals:
    void linesReceived(const QJsonArray& lines);

public:
    void run() override;
    explicit ServerListenerThread(QObject *parent = nullptr);
};

#endif // SERVERLISTENERTHREAD_H
