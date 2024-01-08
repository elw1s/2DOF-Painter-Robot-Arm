#include "ShortestPathWidget.h"
#include <vector>
#include <iostream>
#include <queue>
#include <QEventLoop>
#include <QTimer>


using namespace std;
class Cell {
public:
    int x, y;

    Cell(int x, int y) : x(x), y(y) {}
};

ShortestPathWidget::ShortestPathWidget(QWidget *parent) : QWidget(parent) {
    //setFixedSize(480, 480);
    startFlag = 0;
    destinationFlag = 0;
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);

    // Initialize the grid with 0s (empty cells)
    grid.assign(9, std::vector<int>(9, 0));

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QPushButton *button = new QPushButton("", this);
            button->setFixedSize(60, 60);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setProperty("row", row);
            button->setProperty("col", col);
            button->setText("");
            button->setStyleSheet("background-color: white; border: 1px solid black;");
            connect(button, &QPushButton::clicked, this, &ShortestPathWidget::onCellClicked);
            gridLayout->addWidget(button, row, col);
            cellButtons.push_back(button);
        }
    }

    setLayout(gridLayout);
}


void ShortestPathWidget::onCellClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    QFont font = clickedButton->font();
    font.setPointSize(32);
    clickedButton->setFont(font);
    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();
    selectedCell = QPoint(col, row);

    int number = -1;

    if(clickedButton->text().isEmpty()){
        number = 0;
    }
    else if(clickedButton->text() == "S"){
        number = 1;
    }
    else if(clickedButton->text() == "D"){
        number = 2;
    }
    else if(clickedButton->text() == "O"){
        number = 3;
    }

    qDebug() << "Number is set to be:"  << number;

    // Print number here
    if(number==0 && startFlag == 1){
        number = 1;
    }
    if(number==1 && destinationFlag == 1){
        number = 2;
    }

    QString colorStyleSheet;

    // Set the background color based on the number
    if (number == 0) {
        if(clickedButton->styleSheet().contains("color: red;")){
            destinationFlag = 0;
        }
        grid[selectedCell.y()][selectedCell.x()] = 2;
        colorStyleSheet = "background-color: white; border: 1px solid black; color: green;";
        clickedButton->setText("S");
        startFlag = 1;
    } else if (number == 1 ) {
        if(clickedButton->styleSheet().contains("color: green;")){
            startFlag = 0;
        }
        grid[selectedCell.y()][selectedCell.x()] = 3;
        colorStyleSheet = "background-color: white; border: 1px solid black; color: red;";
        clickedButton->setText("D");
        destinationFlag = 1;
    } else if (number == 2) {
        if(clickedButton->styleSheet().contains("color: red;")){
            destinationFlag = 0;
        }
        if(clickedButton->styleSheet().contains("color: green;")){
            startFlag = 0;
        }
        grid[selectedCell.y()][selectedCell.x()] = 1;
        colorStyleSheet = "background-color: white; border: 1px solid black; color: grey;";
        clickedButton->setText("O");
    } else if (number == 3) {
        if(clickedButton->styleSheet().contains("color: red;")){
            destinationFlag = 0;
        }
        if(clickedButton->styleSheet().contains("color: green;")){
            startFlag = 0;
        }
        grid[selectedCell.y()][selectedCell.x()] = 0;
        colorStyleSheet = "background-color: white; border: 1px solid black; color: black;";
        clickedButton->setText("");
    } else {
        colorStyleSheet = "background-color: white; border: 1px solid black; color: black;";
    }

    clickedButton->setStyleSheet(colorStyleSheet);

    lastClickedButton = clickedButton;
    lastClickedPoint = selectedCell;
}



void ShortestPathWidget::onNumberSelected(int number) {
}

std::pair<int, int> ShortestPathWidget::findStartPosition() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col] == 2) {
                return std::make_pair(row, col);
            }
        }
    }
    // Return a default value (e.g., -1, -1) if not found
    return std::make_pair(-1, -1);
}

std::pair<int, int> ShortestPathWidget::findDestinationPosition() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col] == 3) {
                return std::make_pair(row, col);
            }
        }
    }
    // Return a default value (e.g., -1, -1) if not found
    return std::make_pair(-1, -1);
}


// Check if the cell is valid
bool isValid(int x, int y) {
    return x >= 0 && x < 9 && y >= 0 && y < 9;
}

void removeLastElement(std::queue<Cell>& myQueue) {
    if (myQueue.empty()) {
        // The queue is empty, nothing to remove
        return;
    }

    // Create a temporary queue to hold elements except the last one
    std::queue<Cell> tempQueue;

    // Move elements from the original queue to the temporary queue
    while (myQueue.size() > 1) {
        tempQueue.push(myQueue.front());
        myQueue.pop();
    }

    // Now, myQueue has only the last element remaining
    // Pop the last element from myQueue
    myQueue.pop();

    // Swap the contents of the original queue with the temporary one
    std::swap(myQueue, tempQueue);
}

bool ShortestPathWidget::solve() {


    if(startFlag == 0 || destinationFlag == 0){
        return false;
    }

    // Find the position of the start index
    std::pair<int, int> startPosition = findStartPosition();
    if (startPosition.first != -1 && startPosition.second != -1) {
        qDebug() << "Start Position - Row: " << startPosition.first << " Column: " << startPosition.second;
    } else {
        qDebug() << "Start index not found.";
    }

    // Find the position of the destination index
    std::pair<int, int> destinationPosition = findDestinationPosition();
    if (destinationPosition.first != -1 && destinationPosition.second != -1) {
        qDebug() << "Destination Position - Row: " << destinationPosition.first << " Column: " << destinationPosition.second;
    } else {
        qDebug() << "Destination index not found.";
    }


    // Starting point (0, 0)
    Cell end(startPosition.first, startPosition.second);

    // Ending point (4, 4)
    Cell start(destinationPosition.first,destinationPosition.second);

    // Movement vectors: right, left, down, up
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    // Matrix to keep track of visited cells during BFS
    vector<vector<bool>> visited(9, vector<bool>(9, false));

    // Matrix to keep track of passed cells during BFS
    vector<vector<Cell>> parent(9, vector<Cell>(9, Cell(-1, -1)));

    // Queue used during BFS
    queue<Cell> q;

    // Add the starting cell to the queue
    q.push(start);
    visited[start.x][start.y] = true;



    // BFS algorithm
    while (!q.empty()) {
        Cell current = q.front();
        q.pop();

        // Has the destination been reached?
        if (current.x == end.x && current.y == end.y) {
            // Print the coordinates of the shortest path
            qDebug() << "Coordinates of the shortest path:";

            // Traverse the shortest path
            while (current.x != -1 && current.y != -1) {
                QPushButton *button = cellButtons[current.x * 9 + current.y];
                QFont font = button->font();
                font.setPointSize(32);
                button->setFont(font);
                // Get the current style sheet of the button
                QString currentColor = button->styleSheet();

                // Check if the current color is neither green nor red
                if (!currentColor.contains("color: green;") && !currentColor.contains("color: red;")) {
                    // Change the color of the button to blue
                    button->setStyleSheet("background-color: white; border: 1px solid black; color: blue;");
                    button->setText("X");
                }

                qDebug() << "(" << current.x << ", " << current.y << ")";
                current = parent[current.x][current.y];

                // Introduce a delay of 1 second
                QEventLoop loop;
                QTimer::singleShot(1000, &loop, SLOT(quit()));
                loop.exec();
            }
            return true;
        }

        // Explore movement vectors
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            // Is the new cell valid and has it not been visited before?
            if (isValid(newX, newY) && (grid[newX][newY] == 0 || grid[newX][newY] == 2 || grid[newX][newY] == 3) && !visited[newX][newY]) {
                // Add the cell to the queue
                q.push(Cell(newX, newY));
                visited[newX][newY] = true;
                parent[newX][newY] = current;
            }
        }
    }

    // If the destination is not reached from the starting point
    cout << "Shortest path not found." << endl;
    resetGrid();
    return false;
}

void ShortestPathWidget::resetGrid() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            grid[row][col] = 0; // Resetting each cell to empty
            QPushButton *button = cellButtons[row * 9 + col];
            button->setStyleSheet("background-color: white; border: 1px solid black; color: black;");
            button->setText("");
            QFont font = button->font();
            font.setPointSize(10); // Set font size back to normal
            button->setFont(font);
        }
    }
    startFlag = 0; // Reset start and destination flags
    destinationFlag = 0;
}

void ShortestPathWidget::saveAsImage(const QString &filePath) {
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

