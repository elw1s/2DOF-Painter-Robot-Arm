#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QMessageBox>

class Settings : public QDialog {
    Q_OBJECT

public:
    explicit Settings(const QString& ipAddress, int port, QWidget *parent = nullptr);
    void onOKButtonClicked();
    void onDisconnectButtonClicked();
private:
    QVBoxLayout *mainLayout;
    QVBoxLayout* leftLayout;
    QVBoxLayout* rightLayout;
    QLabel *labelIP;
    QLabel *labelPort;
    QLineEdit *lineEditIP;
    QLineEdit *lineEditPort;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QString mIpAddress;
    int mPort;
signals:
    void settingsUpdated(const QString& ipAddress, int port);
    void disconnectSignal();
};

#endif // SETTINGS_H
