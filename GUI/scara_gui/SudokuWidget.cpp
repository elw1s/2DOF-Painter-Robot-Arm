#include "SudokuWidget.h"


SudokuWidget::SudokuWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(480, 480);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QPushButton *button = new QPushButton("", this);
            button->setFixedSize(50, 50);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setProperty("row", row);
            button->setProperty("col", col);
            if(col == 2 || col == 5 || col == 8){
                button->setStyleSheet("background-color: white; border: 1px solid black; border-right: 2px solid black");
            }
            else if(col == 0){
                button->setStyleSheet("background-color: white; border: 1px solid black; border-left: 2px solid black");
            }
            else{
                button->setStyleSheet("background-color: white; border: 1px solid black;");
            }

            if(row == 0){
                button->setStyleSheet("background-color: white; border: 1px solid black; border-top: 2px solid black");
            }
            else if(row == 2 || row == 5 || row == 8){
                button->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 2px solid black");
            }
            else{
                button->setStyleSheet("background-color: white; border: 1px solid black;");
            }
            connect(button, &QPushButton::clicked, this, &SudokuWidget::onCellClicked);
            gridLayout->addWidget(button, row, col);
            cellButtons.push_back(button);
        }
    }

    setLayout(gridLayout);
}


void SudokuWidget::onCellClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if(selectedCell != QPoint(col,row)){
        QPushButton *lastClickedButton = cellButtons[selectedCell.y() * 9 + selectedCell.x()];
        if(lastClickedButton){
            lastClickedButton->setStyleSheet("background-color: white; border: 1px solid black;");
        }
    }

    selectedCell = QPoint(col, row);

    // Change background color of the clicked cell
    clickedButton->setStyleSheet("background-color: lightblue; border: 1px solid black;");
}

void SudokuWidget::onNumberSelected(int number) {
    QPushButton *clickedButton = cellButtons[selectedCell.y() * 9 + selectedCell.x()];
    if (clickedButton) {
        clickedButton->setText(QString::number(number));
        clickedButton->setStyleSheet("background-color: white; border: 1px solid black;");
    }
}
