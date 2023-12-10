#ifndef SUDOKUAPP_H
#define SUDOKUAPP_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QSpacerItem>
#include <QLabel>
#include <QMovie>
#include "SudokuWidget.h"
#include "ImagePathsConfig.h"


class SudokuApp : public QWidget {
    Q_OBJECT

public:
    SudokuApp(QWidget *parent = nullptr);
private:
    SudokuWidget *sudokuWidget;
    QPushButton *solveButton;
    QGridLayout *numberGrid;
    QVBoxLayout *mainLayout;
    QVBoxLayout *verticalButtonLayout;
    void toggleOverlay(bool showOverlay);
private slots:
    void onNumberButtonClicked();
    void onSolveButtonClicked();

public slots:
    void robotDrawingSignal(const bool status);
signals:
    void drawButtonClicked();
};

#endif // SUDOKUAPP_H
