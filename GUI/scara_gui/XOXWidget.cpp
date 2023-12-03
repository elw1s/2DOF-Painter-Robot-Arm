#include "XOXWidget.h"
#include <QRandomGenerator>

XOXWidget::XOXWidget(QWidget *parent) : QWidget(parent) {

    gridLayout = new QGridLayout(this);
    isUserTurn = true;
    isGameOver = false;

    // Set maximum button size to 30x30
    QSize maxButtonSize(180, 180);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QPushButton *button = new QPushButton("", this);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            // Set maximum size
            button->setMaximumSize(maxButtonSize);

            // Set style for the button
            button->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; }");

            // Set text properties
            button->setFont(QFont("Arial", 14));

            connect(button, &QPushButton::clicked, this, &XOXWidget::buttonClicked);
            gridLayout->addWidget(button, i, j);
            buttons.push_back(button);
        }
    }
    gridLayout->setSpacing(0); // Set spacing between buttons to zero
    // Set the grid layout to be centered horizontally and vertically
    gridLayout->setAlignment(Qt::AlignCenter);

    resetGame();
}

void XOXWidget::resetGame() {
    isUserTurn = true;
    isGameOver = false;
    for (QPushButton *button : buttons) {
        button->setText("");
        button->setEnabled(true);
    }
}

void XOXWidget::buttonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton || !isUserTurn || isGameOver)
        return;

    clickedButton->setText("X");
    clickedButton->setEnabled(false);
    checkGameStatus();

    if (!isGameOver) {
        isUserTurn = false;
        makeComputerMove();
        checkGameStatus();
    }
}

// Function to evaluate the current state of the board
int XOXWidget::evaluateBoard() {
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < 3; ++i) {
        // Check rows
        if (buttons[i * 3]->text() == buttons[i * 3 + 1]->text() &&
            buttons[i * 3]->text() == buttons[i * 3 + 2]->text() &&
            buttons[i * 3]->text() != "") {
            if (buttons[i * 3]->text() == "O") {
                return 10; // Computer wins
            } else {
                return -10; // User wins
            }
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if ((buttons[i]->text() == buttons[i + 3]->text() &&
             buttons[i]->text() == buttons[i + 6]->text() &&
             buttons[i]->text() != "") ||
            (buttons[i * 3]->text() == buttons[i * 3 + 1]->text() &&
             buttons[i * 3]->text() == buttons[i * 3 + 2]->text() &&
             buttons[i * 3]->text() != "") ||
            (buttons[i + 6]->text() == buttons[i + 3]->text() &&
             buttons[i + 6]->text() == buttons[i]->text() &&
             buttons[i + 6]->text() != "")) {
            if (buttons[i]->text() == "O" ||
                buttons[i + 3]->text() == "O" ||
                buttons[i + 6]->text() == "O") {
                return 10; // Computer wins
            } else {
                return -10; // User wins
            }
        }
    }

    // Check diagonals
    if ((buttons[0]->text() == buttons[4]->text() && buttons[0]->text() == buttons[8]->text() && buttons[0]->text() != "") ||
        (buttons[2]->text() == buttons[4]->text() && buttons[2]->text() == buttons[6]->text() && buttons[2]->text() != "")) {
        if (buttons[4]->text() == "O") {
            return 10; // Computer wins
        } else {
            return -10; // User wins
        }
    }

    // If no one has won yet, check for a draw
    for (QPushButton* button : buttons) {
        if (button->text().isEmpty()) {
            return 0; // Game still ongoing
        }
    }

    return 0; // If no one has won and all buttons are filled, it's a draw
}

// Check if a move is legal
bool XOXWidget::isMoveLegal(int move) {
    return buttons[move]->text().isEmpty();
}

// Minimax algorithm implementation
Move XOXWidget::minimax(bool isMaximizing) {
    int score = evaluateBoard();

    // Base case: If the game is over, return the score
    if (score == 10 || score == -10 || evaluateBoard()) {
        return { -1, score };
    }

    // Initialize best move and score
    Move bestMove;
    bestMove.score = isMaximizing ? -1000 : 1000;

    // Loop through all possible moves
    for (int i = 0; i < 9; ++i) {
        if (isMoveLegal(i)) {
            // Try the move
            buttons[i]->setText(isMaximizing ? "O" : "X");

            // Recursively find the best move
            Move currentMove = minimax(!isMaximizing);

            // Undo the move
            buttons[i]->setText("");

            // Update best move and score
            if ((isMaximizing && currentMove.score > bestMove.score) ||
                (!isMaximizing && currentMove.score < bestMove.score)) {
                bestMove.score = currentMove.score;
                bestMove.index = i;
            }
        }
    }

    return bestMove;
}

// Function to make the computer's move using minimax
void XOXWidget::makeComputerMove() {
    if (isGameOver) {
        return;
    }

    // Find the best move using minimax algorithm
    Move bestMove = minimax(true);

    // Make the best move
    if (bestMove.index != -1) {
        buttons[bestMove.index]->setText("O");
        buttons[bestMove.index]->setEnabled(false);
        isUserTurn = true;
        checkGameStatus();

        // After the computer's move, check if the game is over
        if (!isGameOver) {
            isUserTurn = true; // Set back to user's turn
            checkGameStatus(); // Check if the game is over after the AI's move
        }
    }
}

void XOXWidget::printBoard() {
    qDebug() << "Current Board State:";
    for (int i = 0; i < 3; ++i) {
        QString row;
        for (int j = 0; j < 3; ++j) {
            row += buttons[i * 3 + j]->text() + " ";
        }
        qDebug() << row;
    }
}

void XOXWidget::checkGameStatus() {
    QString gameResult;
    printBoard();
    qDebug() << "TABLE:";
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < 3; ++i) {

        qDebug() << buttons[i]->text() << "==" << buttons[i + 3]->text();
        qDebug() << buttons[i]->text() << "==" << buttons[i + 6]->text();
        qDebug() << buttons[i]->text() << "!=" << "where i is " << i;
        // Check rows
        if ((buttons[i * 3]->text() == buttons[i * 3 + 1]->text() &&
             buttons[i * 3]->text() == buttons[i * 3 + 2]->text() &&
             buttons[i * 3]->text() != "")){
            gameResult = buttons[i * 3]->text();
            break;

        }
        else if((buttons[i]->text() == buttons[i + 3]->text() &&
                    buttons[i]->text() == buttons[i + 6]->text() &&
                    buttons[i]->text() != "")){

            gameResult = buttons[i]->text();
            break;
        }
    }

    qDebug() << "The winner is " << gameResult;

    // Check diagonals
    if ((buttons[0]->text() == buttons[4]->text() && buttons[0]->text() == buttons[8]->text() && buttons[0]->text() != "") ||
        (buttons[2]->text() == buttons[4]->text() && buttons[2]->text() == buttons[6]->text() && buttons[2]->text() != "")) {
        gameResult = buttons[4]->text();
    }

    // Check for a draw
    if (gameResult.isEmpty()) {
        bool allButtonsClicked = true;
        for (QPushButton* button : buttons) {
            if (button->text() == "") {
                allButtonsClicked = false;
                break;
            }
        }
        if (allButtonsClicked) {
            gameResult = "DRAW";
        }
    }

    if (!gameResult.isEmpty()) {
        isGameOver = true;
        // Disable buttons
        for (QPushButton* button : buttons) {
            button->setEnabled(false);
        }

        // Update the label text based on the game result
        if (gameResult == "X") {
            emit gameOver(1);
        } else if (gameResult == "O") {
            emit gameOver(2);
        } else {
            emit gameOver(0);
        }
    }
}


