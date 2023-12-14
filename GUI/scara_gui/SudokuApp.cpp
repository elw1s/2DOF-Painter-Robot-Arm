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
//    numberGrid = new QGridLayout();
//    numberGrid->setSpacing(0); // Set spacing between buttons to zero
//    numberGrid->setVerticalSpacing(0); // Set vertical spacing between buttons to zero
//    // Add number buttons to the grid layout
//    for (int i = 1; i <= 9; ++i) {
//        QPushButton *numberButton = new QPushButton(QString::number(i), this);
//        numberButton->setFixedSize(100, 100);
//        numberButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        numberButton->setFont(QFont("Arial", 24));
//        numberButton->setStyleSheet("QPushButton { margin: 0px; padding: 0px; spacing: 0px; background-color: #19749B; border: 1px solid #33C2FF; }");
//        numberButton->setCursor(Qt::PointingHandCursor);

//        connect(numberButton, &QPushButton::clicked, this, &SudokuApp::onNumberButtonClicked);

//        numberGrid->addWidget(numberButton, (i - 1) / 3, (i - 1) % 3);
//    }
    //horizontalLayout->addLayout(numberGrid);

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

    resetButton = new QPushButton("Reset", this);
    resetButton->setObjectName("resetButton"); // Set object name to apply specific styles
    resetButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resetButton->setFixedWidth(200);
    resetButton->setFixedHeight(50); // Set the fixed height
    resetButton->setStyleSheet("QPushButton#resetButton {"
                               "    background-color: #33C2FF;"
                               "    color: #424242;"
                               "    font-family: Abel;"
                               "    font-size: 12px;"
                               "    border: 1px solid #767676;"
                               "    margin-right: 0px;"
                               "    margin-bottom: 5px;"
                               "}"
                               "QPushButton#resetButton:hover {"
                               "    background-color: #57D5FF;" // Change color on hover if desired
                               "}");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Push the button to the right
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(solveButton);

    connect(solveButton, &QPushButton::clicked, this, &SudokuApp::onSolveButtonClicked);
    connect(resetButton, &QPushButton::clicked, this, &SudokuApp::onResetButtonClicked);
    verticalButtonLayout->addLayout(buttonLayout);

    mainLayout->setAlignment(Qt::AlignRight | Qt::AlignTop); // Align the layout to the top-right

    // Create QLabel for title
    QLabel *titleLabel = new QLabel("How to play?", this);
    titleLabel->setFont(QFont("Arial", 20, QFont::Bold)); // Setting font size and weight
    titleLabel->setFixedSize(400, titleLabel->sizeHint().height()); // Set fixed width and height based on text
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align text to the top-left
    titleLabel->setStyleSheet("color: white;");
    // Create QLabel for description
    QLabel *descriptionLabel = new QLabel("Select a cell on the 9x9 grid below. Enter a number \nusing keyboard. To solve the sudoku, \npress \"Solve\" button.", this);
    descriptionLabel->setFont(QFont("Arial", 14)); // Setting font size
    descriptionLabel->setFixedSize(480, descriptionLabel->sizeHint().height()); // Set fixed width and height based on text
    descriptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align text to the top-left
    descriptionLabel->setStyleSheet("color: white;");
    // Create a vertical layout for the title and description labels
    QVBoxLayout *labelLayout = new QVBoxLayout();
    labelLayout->addWidget(titleLabel);
    labelLayout->addWidget(descriptionLabel);
    labelLayout->setSpacing(0); // Add some spacing between the labels

    mainLayout->addLayout(labelLayout);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(verticalButtonLayout);
    setLayout(mainLayout);

    this->installEventFilter(this);
}

bool SudokuApp::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int keyPressed = keyEvent->key() - Qt::Key_0; // Convert key code to number

        // Check if the pressed key is a number between 1 to 9
        if (keyPressed >= 1 && keyPressed <= 9) {
            sudokuWidget->onNumberSelected(keyPressed);
            return true; // Event handled
        }
    }

    // Pass the event to the base class
    return QObject::eventFilter(obj, event);
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
        qDebug() << "Sudoku has no solution..";
        // If the puzzle has no solution, handle this scenario
    }
}

void SudokuApp::onResetButtonClicked() {
    sudokuWidget->resetBoard();
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
        resetButton->setEnabled(true);
        resetButton->setStyleSheet("QPushButton#resetButton {"
                                   "    background-color: #33C2FF;"
                                   "    color: #424242;"
                                   "    font-family: Abel;"
                                   "    font-size: 12px;"
                                   "    border: 1px solid #767676;"
                                   "    margin-right: 0px;"
                                   "    margin-bottom: 5px;"
                                   "}"
                                   "QPushButton#resetButton:hover {"
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
        resetButton->setEnabled(false);
        resetButton->setStyleSheet("QPushButton#resetButton {"
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

