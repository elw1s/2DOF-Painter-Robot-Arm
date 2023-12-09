#include "SudokuApp.h"
#include <QHBoxLayout> // Include the header file for QHBoxLayout

SudokuApp::SudokuApp(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Horizontal layout for sudokuWidget and numberGrid
    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    sudokuWidget = new SudokuWidget(this);
    horizontalLayout->addWidget(sudokuWidget);

    // Create the grid layout for number buttons
    numberGrid = new QGridLayout();
    numberGrid->setSpacing(0); // Set spacing between buttons to zero
    numberGrid->setVerticalSpacing(0); // Set vertical spacing between buttons to zero
    // Add number buttons to the grid layout
    for (int i = 1; i <= 9; ++i) {
        QPushButton *numberButton = new QPushButton(QString::number(i), this);
        numberButton->setFixedSize(100, 100);
        numberButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        numberButton->setFont(QFont("Arial", 24));
        numberButton->setStyleSheet("QPushButton { margin: 0px; padding: 0px; spacing: 0px; background-color: #D3D3D3; border: 1px solid black; }");
        numberButton->setCursor(Qt::PointingHandCursor);

        connect(numberButton, &QPushButton::clicked, this, &SudokuApp::onNumberButtonClicked);

        numberGrid->addWidget(numberButton, (i - 1) / 3, (i - 1) % 3);
    }
    horizontalLayout->addLayout(numberGrid);

    // Vertical layout for the solveButton
    QVBoxLayout *verticalButtonLayout = new QVBoxLayout();
    solveButton = new QPushButton("Solve", this);
    solveButton->setFixedSize(100, 30);
    solveButton->setFont(QFont("Arial", 10));
    solveButton->setStyleSheet("background-color: #4CAF50; color: white; border: none;");
    solveButton->setCursor(Qt::PointingHandCursor);
    connect(solveButton, &QPushButton::clicked, this, &SudokuApp::onSolveButtonClicked);

    verticalButtonLayout->addWidget(solveButton, 0, Qt::AlignCenter);

    // Add the layouts to the main layout
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(verticalButtonLayout);

    setLayout(mainLayout);
}

void SudokuApp::onNumberButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    int selectedNumber = clickedButton->text().toInt();
    sudokuWidget->onNumberSelected(selectedNumber);
}

void SudokuApp::onSolveButtonClicked() {
    bool isSolved = sudokuWidget->solveSudoku();
    if (isSolved) {
        QString filePath = "/home/ardakilic/Desktop/CSE396/GUI/scara_gui/GameBoards/sudoku/solved.jpg";
        sudokuWidget->saveAsImage(filePath);
        // Puzzle is solved
        // You can perform additional actions here if needed
    } else {
        // If the puzzle has no solution, handle this scenario
    }
}
