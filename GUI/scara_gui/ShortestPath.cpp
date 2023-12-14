#include "ShortestPath.h"

ShortestPath::ShortestPath(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    buttonsAndWidgetLayout = new QHBoxLayout(this);
    shortestPathWidget = new ShortestPathWidget(this);
    buttonsAndWidgetLayout->addWidget(shortestPathWidget, 0, Qt::AlignCenter);

    // Create QLabel for title
    QLabel *titleLabel = new QLabel("How to play?", this);
    titleLabel->setFont(QFont("Arial", 20, QFont::Bold)); // Setting font size and weight
    titleLabel->setFixedSize(400, titleLabel->sizeHint().height()); // Set fixed width and height based on text
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align text to the top-left
    titleLabel->setStyleSheet("color: white;");
    // Create QLabel for description
    QLabel *descriptionLabel = new QLabel("Select a cell by clicking on the 9x9 grid below.\nEach click will be different.\nFirst click: START,\nSecond click: DESTINATION\nThird click: OBSTACLE\nFourth: EMPTY\nThe found result will be drawn by robot.\n", this);
    descriptionLabel->setFont(QFont("Arial", 14)); // Setting font size
    descriptionLabel->setFixedSize(480, descriptionLabel->sizeHint().height()); // Set fixed width and height based on text
    descriptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align text to the top-left
    descriptionLabel->setStyleSheet("color: white;");
    // Create a vertical layout for the title and description labels
    QVBoxLayout *labelLayout = new QVBoxLayout();
    labelLayout->addWidget(titleLabel);
    labelLayout->addWidget(descriptionLabel);
    labelLayout->setSpacing(0); // Add some spacing between the labels


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

    connect(solveButton, &QPushButton::clicked, this, &ShortestPath::onSolveButtonClicked);
    connect(resetButton, &QPushButton::clicked, shortestPathWidget, &ShortestPathWidget::resetGrid);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Push the button to the right
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(solveButton);

    mainLayout->addLayout(labelLayout);
    mainLayout->addLayout(buttonsAndWidgetLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void ShortestPath::onNumberButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    int selectedNumber = -1;  // Default value for buttons not associated with numbers

    if (clickedButton->text() == "Set Start") {
        selectedNumber = 0;
    } else if (clickedButton->text() == "Set Destination") {
        selectedNumber = 1;
    } else if (clickedButton->text() == "Add Obstacle") {
        selectedNumber = 2;
    } else if (clickedButton->text() == "Set Empty") {
        selectedNumber = 3;
    }

    shortestPathWidget->onNumberSelected(selectedNumber);
}

void ShortestPath::onSolveButtonClicked() {
    bool isSolved = shortestPathWidget->solve();
    if(isSolved){
        QDir dir;
        dir = dir.temp();
        dir.setPath(dir.path()+ "/cse396");
        QString filePath = dir.path() + "/image.jpg";
        shortestPathWidget->saveAsImage(filePath);
        emit drawButtonClicked();
    }
    else{

    }
}

void ShortestPath::robotDrawingSignal(const bool status){
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

void ShortestPath::toggleOverlay(bool showOverlay) {
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

