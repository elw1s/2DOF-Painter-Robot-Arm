#include "SudokuApp.h"
#include <QHBoxLayout> // Include the header file for QHBoxLayout
#include <QDir>

SudokuApp::SudokuApp(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);
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
        numberButton->setStyleSheet("QPushButton { margin: 0px; padding: 0px; spacing: 0px; background-color: #19749B; border: 1px solid #33C2FF; }");
        numberButton->setCursor(Qt::PointingHandCursor);

        connect(numberButton, &QPushButton::clicked, this, &SudokuApp::onNumberButtonClicked);

        numberGrid->addWidget(numberButton, (i - 1) / 3, (i - 1) % 3);
    }
    horizontalLayout->addLayout(numberGrid);

    // Vertical layout for the solveButton
    verticalButtonLayout = new QVBoxLayout();
    solveButton = new QPushButton("Solve", this);
    solveButton->setObjectName("saveButton"); // Set object name to apply specific styles
    solveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    solveButton->setFixedWidth(200);
    solveButton->setFixedHeight(50); // Set the fixed height
    solveButton->setStyleSheet("QPushButton#saveButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 0px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#saveButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Push the button to the right
    buttonLayout->addWidget(solveButton);

    connect(solveButton, &QPushButton::clicked, this, &SudokuApp::onSolveButtonClicked);    
    verticalButtonLayout->addLayout(buttonLayout);

    mainLayout->setAlignment(Qt::AlignRight | Qt::AlignTop); // Align the layout to the top-right

    // Add the layouts to the main layout
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch();
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
        QDir dir;
        dir = dir.temp();
        dir.setPath(dir.path()+ "/cse396");
        QString filePath = dir.path() + "/image.jpg";
        sudokuWidget->saveAsImage(filePath);
        emit drawButtonClicked();
        // Puzzle is solved
        // You can perform additional actions here if needed
    } else {
        // If the puzzle has no solution, handle this scenario
    }
}

//Freeze the app
void SudokuApp::robotDrawingSignal(const bool status){
    if(status){
        toggleOverlay(false);
        this->setEnabled(false);
        qDebug() << "Robot is drawing...";
    }

    else{
        toggleOverlay(true);
        this->setEnabled(true);
        qDebug() << "Robot is NOT drawing...";
    }
}


void SudokuApp::toggleOverlay(bool showOverlay) {
    if(showOverlay){
        solveButton->setEnabled(true);
        solveButton->setStyleSheet("QPushButton#saveButton {"
                                   "    background-color: #33C2FF;"
                                   "    color: #424242;"
                                   "    font-family: Abel;"
                                   "    font-size: 12px;"
                                   "    border: 1px solid #767676;"
                                   "    margin-right: 0px;"
                                   "    margin-bottom: 5px;"
                                   "}"
                                   "QPushButton#saveButton:hover {"
                                   "    background-color: #57D5FF;" // Change color on hover if desired
                                   "}");
    }
    else{
        solveButton->setEnabled(false);
        solveButton->setStyleSheet("QPushButton#saveButton {"
                                   "    background-color: #4F4F4F;"
                                   "    color: #DEDEDE;"
                                   "    font-family: Abel;"
                                   "    font-size: 12px;"
                                   "    border: 1px solid #767676;"
                                   "    margin-right: 0px;"
                                   "    margin-bottom: 5px;"
                                   "}"
                                   );
    }
}

