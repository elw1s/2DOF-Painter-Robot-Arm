#include "SudokuApp.h"

SudokuApp::SudokuApp(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    sudokuWidget = new SudokuWidget(this);
    mainLayout->addWidget(sudokuWidget, 0, Qt::AlignCenter);

    solveButton = new QPushButton("Solve", this);
    solveButton->setFixedSize(100, 30);
    solveButton->setFont(QFont("Arial", 10));
    solveButton->setStyleSheet("background-color: #4CAF50; color: white; border: none;");
    solveButton->setCursor(Qt::PointingHandCursor);

    mainLayout->addWidget(solveButton, 0, Qt::AlignCenter);

    // Create the grid layout for number buttons
    numberGrid = new QGridLayout();
    mainLayout->addLayout(numberGrid);

    // Add number buttons to the grid layout
    for (int i = 1; i <= 9; ++i) {
        QPushButton *numberButton = new QPushButton(QString::number(i), this);
        numberButton->setFixedSize(50, 50);
        numberButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        numberButton->setFont(QFont("Arial", 12));
        numberButton->setStyleSheet("background-color: #D3D3D3; border: 1px solid black;");
        numberButton->setCursor(Qt::PointingHandCursor);

        connect(numberButton, &QPushButton::clicked, this, &SudokuApp::onNumberButtonClicked);

        numberGrid->addWidget(numberButton, (i - 1) / 3, (i - 1) % 3);
    }

    setLayout(mainLayout);
}

void SudokuApp::onNumberButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    int selectedNumber = clickedButton->text().toInt();
    sudokuWidget->onNumberSelected(selectedNumber);
}
