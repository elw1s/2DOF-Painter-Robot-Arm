#ifndef NUMBERSELECTIONDIALOG_H
#define NUMBERSELECTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QStyle>

class NumberSelectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit NumberSelectionDialog(QWidget *parent = nullptr);

signals:
    void numberSelected(int number);

private slots:
    void onNumberClicked();

private:
    void createButtons();
};

#endif // NUMBERSELECTIONDIALOG_H
