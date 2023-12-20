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
#include <QColorDialog>

class Settings : public QDialog {
    Q_OBJECT

public:
    explicit Settings(const QString& ipAddress, int port,const QList<QString>& colorArr, QWidget *parent = nullptr);
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
    QPushButton* colorButtons[4];
    QList<QString> colors;
signals:
    void settingsUpdated(const QString& ipAddress, int port);
    void disconnectSignal();
    void setColors(const QList<QString>& colorArr);
private slots:
    void onColorButtonClicked(int buttonIndex);
    void updateButtonColor(int buttonIndex, const QColor& color); // Slot to update button color
};

#endif // SETTINGS_H
