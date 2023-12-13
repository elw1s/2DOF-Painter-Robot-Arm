#ifndef SUDOKUWIDGET_H
#define SUDOKUWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include <QPushButton>
#include <QFrame>
#include "NumberSelectionDialog.h"
#include <QFile>
#include <QMessageBox>
#include <QTimer>

class SudokuWidget : public QWidget {
    Q_OBJECT

public:
    explicit SudokuWidget(QWidget *parent = nullptr);
    void onNumberSelected(int number);
    bool solveSudoku();
    void saveAsImage(const QString &filePath);
    void resetBoard();
private:
    QPoint selectedCell; // Stores the selected cell
    std::vector<QPushButton*> cellButtons;
    QGridLayout *gridLayout;
    int selectedButtonIndex;
    bool findEmptyCell(int &row, int &col);
    bool isValid(int row, int col, int num);
    bool isInRow(int row, int num) const;
    bool isInCol(int col, int num) const;
    bool isInBox(int startRow, int startCol, int num) const;
private slots:
    void onCellClicked();
};

#endif // SUDOKUWIDGET_H
