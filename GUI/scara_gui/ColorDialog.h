#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include <QColor>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class ColorDialog : public QDialog {
    Q_OBJECT

public:
    ColorDialog(const QColor &initialColor, const QString &labelText, QWidget *parent = nullptr);

private slots:
    void onOKButtonClicked();

private:
    void setupUI();

    QLabel *colorSquare;
    QPushButton *okButton;
    QLabel *textLabel;
};

#endif // COLORDIALOG_H
