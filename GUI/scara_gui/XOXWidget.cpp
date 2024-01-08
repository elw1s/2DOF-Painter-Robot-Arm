#include "XOXWidget.h"
#include <QRandomGenerator>

XOXWidget::XOXWidget(QWidget *parent) : QWidget(parent) {

    gridLayout = new QGridLayout(this);
    isUserTurn = true;
    isGameOver = false;
    saveWithBoard = true;

    // Set maximum button size to 30x30
    QSize maxButtonSize(180, 180);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QPushButton *button = new QPushButton("", this);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            // Set maximum size
            button->setMaximumSize(maxButtonSize);

            // Set style for the button
            button->setStyleSheet("QPushButton { color:black; background-color: white; border: 1px solid black; }");

            // Set text properties
            button->setFont(QFont("Arial", 30));

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
    if (!clickedButton || isGameOver) // Check if the game is over before processing
        return;

    if (isUserTurn) { // Only allow user moves if it's the user's turn
        if (clickedButton->text().isEmpty()) { // Check if the button is empty
            clickedButton->setText("X");
            clickedButton->setEnabled(false);
            lastMoveX = clickedButton;
            checkGameStatus();
            if (!isGameOver) {
                isUserTurn = false;
                makeComputerMove(); // Allow AI move only if the game is still ongoing
                checkGameStatus();
            }
            saveMoveImage(saveWithBoard);
            saveWithBoard = false;
        }
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
int XOXWidget::minimax(bool isMaximizing, int depth, int alpha, int beta) {
    int score = evaluateBoard();

    // Base case: If the game is over or maximum depth reached, return the score
    if (score == 10 || score == -10 || depth == 0 || evaluateBoard()) {
        return score;
    }

    // Maximizing player's turn
    if (isMaximizing) {
        int bestVal = INT_MIN;

        for (int i = 0; i < 9; ++i) {
            if (isMoveLegal(i)) {
                // Try the move
                buttons[i]->setText("O");

                // Recursively find the best move
                bestVal = std::max(bestVal, minimax(!isMaximizing, depth - 1, alpha, beta));

                // Undo the move
                buttons[i]->setText("");

                // Update alpha value
                alpha = std::max(alpha, bestVal);

                if (beta <= alpha) {
                    break; // Beta cut-off
                }
            }
        }
        return bestVal;
    } else { // Minimizing player's turn
        int bestVal = INT_MAX;

        for (int i = 0; i < 9; ++i) {
            if (isMoveLegal(i)) {
                // Try the move
                buttons[i]->setText("X");

                // Recursively find the best move
                bestVal = std::min(bestVal, minimax(!isMaximizing, depth - 1, alpha, beta));

                // Undo the move
                buttons[i]->setText("");

                // Update beta value
                beta = std::min(beta, bestVal);

                if (beta <= alpha) {
                    break; // Alpha cut-off
                }
            }
        }
        return bestVal;
    }
}

void XOXWidget::applyBorder(const QString filePath) {
    QImage image(filePath); // Load the image
    if (image.isNull()) {
        qDebug() << "Failed to load the image.";
        return; // Exit or handle the failure
    }

    // Define the size of the border (in pixels)
    int borderWidth = 10;

    // Create a painter to draw on the image
    QPainter painter(&image);
    painter.setPen(QPen(Qt::black, 4)); // Set the pen color to black and width to 4 pixels
    painter.setRenderHint(QPainter::Antialiasing); // Optional: Enable anti-aliasing for smoother lines

    // Draw a rectangle border around the image
    painter.drawRect(borderWidth, borderWidth, image.width() - 2 * borderWidth - 4, image.height() - 2 * borderWidth - 4);

    if (!image.save(filePath)) {
        qDebug() << "Failed to save the framed image.";
        return; // Exit or handle the failure
    }
}

void XOXWidget::saveMoveImage(bool board) {

    QImage image(1024, 1024, QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setFont(QFont("Arial", 40));
    QPen pen(Qt::black);
    pen.setWidth(4);
    painter.setPen(pen);

    //painter.translate(-500, 0);
    painter.translate(-200, 0); // Shift the image to the left by 100 pixels

    // Draw button borders and marks if board is true
    if (board) {
        for (QPushButton *button : buttons) {
            QPoint buttonPos = button->mapTo(this, QPoint(0, 0));
            QRect buttonRect(buttonPos, button->size());

            // Draw the borders
            painter.drawRect(buttonRect);

            // Draw X and O within the button borders
            if (button == lastMoveX || button == lastMoveO) {
                QString text = (button == lastMoveX) ? "X" : "O";
                painter.drawText(buttonRect, Qt::AlignCenter, text);
            }
        }
    }

    // Draw lastMoveX and lastMoveO only if board is false
    else if (lastMoveX != nullptr) {
        for (QPushButton *button : { lastMoveX, lastMoveO }) {
            QPoint buttonPos = button->mapTo(this, QPoint(0, 0));
            QRect buttonRect(buttonPos, button->size());

            // Draw X and O without borders
            QString text = (button == lastMoveX) ? "X" : "O";
            painter.drawText(buttonRect, Qt::AlignCenter, text);
        }
    }

    QDir dir;
    dir = dir.temp();
    dir.setPath(dir.path() + "/cse396");
    QString filePath = dir.path() + "/image.jpg";
    image.save(filePath);
    applyBorder(filePath);
    emit drawButtonClicked();
}

Move XOXWidget::findBestMove() {
    int bestVal = INT_MIN;
    Move bestMove;
    bestMove.score = INT_MIN;

    for (int i = 0; i < 9; ++i) {
        if (isMoveLegal(i)) {
            // Try the move
            buttons[i]->setText("O");

            // Get the evaluation score using minimax
            int moveVal = minimax(false, /* Depth */ 5, /* Alpha */ INT_MIN, /* Beta */ INT_MAX);

            // Undo the move
            buttons[i]->setText("");

            // Update the best move if needed
            if (moveVal > bestVal) {
                bestVal = moveVal;
                bestMove.index = i;
                bestMove.score = moveVal;
            }
        }
    }
    return bestMove;
}

void XOXWidget::makeComputerMove() {
    if (isGameOver) {
        return;
    }

    // Find the best move using minimax algorithm
    Move bestMove = findBestMove();

    checkGameStatus();
//    if(isGameOver){
//        saveMoveImage(saveWithBoard);
//        saveWithBoard = false;
//    }

    // Make the best move
    if (bestMove.index != -1) {
        buttons[bestMove.index]->setText("O");
        buttons[bestMove.index]->setEnabled(false);
        lastMoveO = buttons[bestMove.index];
        isUserTurn = true;
        checkGameStatus();
        // After the computer's move, check if the game is over
        if (!isGameOver) {
            isUserTurn = true; // Set back to user's turn
            checkGameStatus(); // Check if the game is over after the AI's move
        }
//        saveMoveImage(saveWithBoard);
//        saveWithBoard = false;
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


