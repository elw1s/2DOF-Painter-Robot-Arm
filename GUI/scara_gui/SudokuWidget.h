#ifndef SUDOKUWIDGET_H
#define SUDOKUWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include <QPushButton>

#include "NumberSelectionDialog.h"

class SudokuWidget : public QWidget {
    Q_OBJECT

public:
    explicit SudokuWidget(QWidget *parent = nullptr);
    void onNumberSelected(int number);
private:
    QPoint selectedCell; // Stores the selected cell
    std::vector<QPushButton*> cellButtons;
    int selectedButtonIndex;
private slots:
    void onCellClicked();
};

#endif // SUDOKUWIDGET_H
