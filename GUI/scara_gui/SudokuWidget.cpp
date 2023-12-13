#include "SudokuWidget.h"


SudokuWidget::SudokuWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(580, 580);

    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QPushButton *button = new QPushButton("", this);
            button->setFixedSize(60, 60);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setProperty("row", row);
            button->setProperty("col", col);

            button->setFont(QFont("Arial", 45));

            if((row == 2 && col == 2) || (row == 2 && col == 5) || (row == 5 && col == 2) || (row == 5 && col == 5)){
                button->setStyleSheet("color: black; background-color: white; margin-bottom: 3px; margin-right: 3px; border: 1px solid black;");
            }
            else if(row == 2 || row == 5){
                button->setStyleSheet("color: black; background-color: white; margin-bottom: 3px; border: 1px solid black;");
            }
            else if(col == 2 || col == 5){
                button->setStyleSheet("color: black; background-color: white; margin-right: 3px; border: 1px solid black;");
            }
            else{
                button->setStyleSheet("color: black; background-color: white; border: 1px solid black;");
            }


            connect(button, &QPushButton::clicked, this, &SudokuWidget::onCellClicked);
            gridLayout->addWidget(button, row, col);
            cellButtons.push_back(button);
        }
    }


    selectedCell.setX(-1);
    selectedCell.setY(-1);

    setLayout(gridLayout);
}

void SudokuWidget::onCellClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if(selectedCell != QPoint(-1,-1) && selectedCell != QPoint(col,row)){
        QPushButton *lastClickedButton = cellButtons[selectedCell.y() * 9 + selectedCell.x()];
        if(lastClickedButton){
            qDebug() << "LastClickedButton row:"<< selectedCell.y() << " col:" << selectedCell.x();
            if((selectedCell.y() == 2 && selectedCell.x() == 2) || (selectedCell.y() == 2 && selectedCell.x() == 5) || (selectedCell.y() == 5 && selectedCell.x() == 2) || (selectedCell.y() == 5 && selectedCell.x() == 5)){
                qDebug() << "The LastClickedButton is BR";
                lastClickedButton->setStyleSheet("color: black; background-color: white; margin-bottom: 3px; margin-right: 3px; border: 1px solid black;");
            }
            else if(selectedCell.y() == 2 || selectedCell.y() == 5){
                qDebug() << "The LastClickedButton is B";
                lastClickedButton->setStyleSheet("color: black; background-color: white; margin-bottom: 3px; border: 1px solid black;");
            }
            else if(selectedCell.x() == 2 || selectedCell.x() == 5){
                qDebug() << "The LastClickedButton is R";
                lastClickedButton->setStyleSheet("color: black; background-color: white; margin-right: 3px; border: 1px solid black;");
            }
            else{
                qDebug() << "The LastClickedButton is Default";
                lastClickedButton->setStyleSheet("color: black; background-color: white; border: 1px solid black;");
            }
        }
    }

    selectedCell = QPoint(col, row);

    qDebug() << "clickedButton row:"<< row << " col:" << col;



    if((row == 2 && col == 2) || (row == 2 && col == 5) || (row == 5 && col == 2) || (row == 5 && col == 5)){
        qDebug() << "The clickedButton is BR";
        clickedButton->setStyleSheet("color: black; background-color: lightblue; margin-bottom: 3px; margin-right: 3px; border: 1px solid black;");
    }
    else if(row == 2 || row == 5){
        qDebug() << "The clickedButton is B";
        clickedButton->setStyleSheet("color: black; background-color: lightblue; margin-bottom: 3px; border: 1px solid black;");
    }
    else if(col == 2 || col == 5){
        qDebug() << "The clickedButton is R";
        clickedButton->setStyleSheet("color: black; background-color: lightblue; margin-right: 3px; border: 1px solid black;");
    }
    else{
        qDebug() << "The clickedButton is Default";
        clickedButton->setStyleSheet("color: black; background-color: lightblue; border: 1px solid black;");
    }
}

void SudokuWidget::onNumberSelected(int number) {
    if(selectedCell.y() == -1 && selectedCell.x() == -1) return;
    QPushButton *clickedButton = cellButtons[selectedCell.y() * 9 + selectedCell.x()];
    if (clickedButton && isValid(selectedCell.y(), selectedCell.x(), number)) {
        clickedButton->setText(QString::number(number));
        //clickedButton->setStyleSheet("color: black; background-color: white; border: 1px solid black;");
        // Find the position of the clickedButton in the gridLayout
        int index = selectedCell.y() * 9 + selectedCell.x();
        qDebug() << "SelectedCell Y = " << selectedCell.y() << " , SelectedCell X = " << selectedCell.x();
        if ((index == 20) || (index == 23) || (index == 47) || (index == 50)) {
            clickedButton->setStyleSheet("color: black; background-color: white; margin-bottom: 3px; margin-right: 3px;");
        } else if ((index / 9 == 2) || (index / 9 == 5)) {
            clickedButton->setStyleSheet("color: black; background-color: white; margin-bottom: 3px;");
        } else if ((index % 9 == 2) || (index % 9 == 5)) {
            clickedButton->setStyleSheet("color: black; background-color: white; margin-right: 3px;");
        } else {
            clickedButton->setStyleSheet("color: black; background-color: white;");
        }
    }
    else{
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("The number is not valid");
        // Set style sheet for the message box
        msgBox->setStandardButtons(QMessageBox::NoButton);
        msgBox->setStyleSheet("QMessageBox { background-color: #19749B; }"
                              "QLabel { color: white; background-color: #19749B; }"
                              "QIcon {background-color: #19749B;}"
                              ); // Change colors

        msgBox->show();

        QTimer::singleShot(500, [msgBox]() {
            if (msgBox && msgBox->isVisible()) {
                msgBox->close();
                msgBox->deleteLater(); // Ensure proper cleanup
            }
        });
    }
}

bool SudokuWidget::isInRow(int row, int num) const {
    for (int col = 0; col < 9; ++col) {
        if (cellButtons[row * 9 + col]->text().toInt() == num) {
            return true;
        }
    }
    return false;
}

bool SudokuWidget::isInCol(int col, int num) const {
    for (int row = 0; row < 9; ++row) {
        if (cellButtons[row * 9 + col]->text().toInt() == num) {
            return true;
        }
    }
    return false;
}

bool SudokuWidget::isInBox(int startRow, int startCol, int num) const {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (cellButtons[(startRow + row) * 9 + startCol + col]->text().toInt() == num) {
                return true;
            }
        }
    }
    return false;
}

bool SudokuWidget::isValid(int row, int col, int num) {
    // Check if the current number is not already present in the row, column, or 3x3 grid
    return !isInRow(row, num) && !isInCol(col, num) && !isInBox(row - row % 3, col - col % 3, num);
}

bool SudokuWidget::solveSudoku() {

    int row, col;

    // Find the next empty cell
    if (!findEmptyCell(row, col)) {
        // If no empty cell is found, the puzzle is solved
        return true;
    }

    // Try placing numbers from 1 to 9 in the current cell
    for (int num = 1; num <= 9; ++num) {
        if (isValid(row, col, num)) {
            // If the current number is valid, place it in the cell
            cellButtons[row * 9 + col]->setText(QString::number(num));

            int index = row * 9 + col;
            if ((index == 20) || (index == 23) || (index == 47) || (index == 50)) {
                cellButtons[index]->setStyleSheet("color: red; background-color: white; margin-bottom: 3px; margin-right: 3px;");
            } else if ((row == 2 || row == 5) && (col == 2 || col == 5)) {
                cellButtons[index]->setStyleSheet("color: red; background-color: white; margin-bottom: 3px; margin-right: 3px;");
            } else if (row == 2 || row == 5) {
                cellButtons[index]->setStyleSheet("color: red; background-color: white; margin-bottom: 3px;");
            } else if (col == 2 || col == 5) {
                cellButtons[index]->setStyleSheet("color: red; background-color: white; margin-right: 3px;");
            } else {
                cellButtons[index]->setStyleSheet("color: red; background-color: white;");
            }
            // Recursively attempt to solve the puzzle with the current number in the cell
            if (solveSudoku()) {
                return true; // If the puzzle is solved with this number, return true
            }

            // If the current number doesn't lead to a solution, backtrack and try the next number
            cellButtons[row * 9 + col]->setText(""); // Clear the cell
        }
    }

    return false; // If no number from 1 to 9 works in this cell, backtrack
}

bool SudokuWidget::findEmptyCell(int &row, int &col) {
    // Find the next empty cell
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            if (cellButtons[row * 9 + col]->text() == "") {
                return true; // Found an empty cell
            }
        }
    }
    return false; // No empty cell found
}

void SudokuWidget::saveAsImage(const QString &filePath) {
    qDebug() << "Inside saveAsImage";

    // Check if the file path is valid
    if (filePath.isEmpty()) {
        qDebug() << "File path is empty.";
        return; // Exit the function early
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file for writing:" << filePath;
        return; // Exit the function early
    }

    // Create a pixmap to hold the grid layout contents
    QPixmap pixmap(this->size());
    pixmap.fill(Qt::white); // Fill the pixmap with a white background (or any other desired background color)

    // Create a painter to draw on the pixmap
    QPainter painter(&pixmap);
    gridLayout->parentWidget()->render(&painter);

    // Save the pixmap as a JPG image
    if (!pixmap.save(filePath, "JPG", 100)) {
        qDebug() << "Failed to save image at path:" << filePath;
        return; // Exit the function early
    }

    qDebug() << "Image saved successfully at:" << filePath;
}

void SudokuWidget::resetBoard() {
    for (QPushButton *button : cellButtons) {
        int row = button->property("row").toInt();
        int col = button->property("col").toInt();

        // Apply styles based on the button's position
        if ((row == 2 && col == 2) || (row == 2 && col == 5) || (row == 5 && col == 2) || (row == 5 && col == 5)) {
            button->setStyleSheet("color: black; background-color: white; border: 1px solid black; margin-bottom: 3px; margin-right: 3px;");
        } else if (row == 2 || row == 5) {
            button->setStyleSheet("color: black; background-color: white; border: 1px solid black; margin-bottom: 3px;");
        } else if (col == 2 || col == 5) {
            button->setStyleSheet("color: black; background-color: white; border: 1px solid black; margin-right: 3px;");
        } else {
            button->setStyleSheet("color: black; background-color: white; border: 1px solid black;");
        }

        // Clear the text of the button
        button->setText("");
    }
}

