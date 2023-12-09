#ifndef SUDOKUAPP_H
#define SUDOKUAPP_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QSpacerItem>

#include "SudokuWidget.h"


class SudokuApp : public QWidget {
    Q_OBJECT

public:
    SudokuApp(QWidget *parent = nullptr);
    void setRightWidget(QWidget *rightWidget);
private:
    SudokuWidget *sudokuWidget;
    QPushButton *solveButton;
    QGridLayout *numberGrid;
    QWidget *overlayWidget;
    void toggleOverlay(bool showOverlay);
    QWidget *m_rightWidget;
private slots:
    void onNumberButtonClicked();
    void onSolveButtonClicked();

public slots:
    void robotDrawingSignal(const bool status);
};

#endif // SUDOKUAPP_H
