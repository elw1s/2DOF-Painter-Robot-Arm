#include "XOXApp.h"

XOXApp::XOXApp(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this); // Create a QVBoxLayout for XOXApp

    gameStatusLabel = new QLabel("Game started", this); // Create the gameStatusLabel
    gameStatusLabel->setAlignment(Qt::AlignHCenter);
    gameStatusLabel->setStyleSheet("color: white;"); // Set text color to white
    gameStatusLabel->setFont(QFont("Arial", 40)); // Set font size to 30
    layout->addWidget(gameStatusLabel); // Add gameStatusLabel to the layout

    gameWidget = new XOXWidget(this);
    connect(gameWidget, &XOXWidget::gameOver, this, &XOXApp::gameFinished);
    connect(gameWidget, &XOXWidget::drawButtonClicked, this, &XOXApp::drawMove);
    layout->addWidget(gameWidget); // Add gameWidget to the layout

    resetButton = new QPushButton("Restart", this);
    resetButton->setObjectName("saveButton"); // Set object name to apply specific styles
    resetButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resetButton->setFixedWidth(200);
    resetButton->setFixedHeight(50); // Set the fixed height
    resetButton->setStyleSheet("QPushButton#saveButton {"
                               "    background-color: #4F4F4F;"
                               "    color: #DEDEDE;"
                               "    font-family: Abel;"
                               "    font-size: 12px;"
                               "    border: 1px solid #767676;"
                               "    margin-right: 0px;"
                               "    margin-bottom: 5px;"
                               "}"
                               );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Push the button to the right
    buttonLayout->addWidget(resetButton);
    layout->addLayout(buttonLayout);
    resetButton->setEnabled(false);
    connect(resetButton, &QPushButton::clicked, this, &XOXApp::resetTable);

    layout->setSpacing(0);
    setLayout(layout); // Set the layout for XOXApp
}

void XOXApp::gameFinished(int result) {
    if(result == 0){
        gameStatusLabel->setText("DRAW!");
    }
    else if(result == 1){
        gameStatusLabel->setText("USER WON!");
    }
    else if(result == 2){
        gameStatusLabel->setText("COMPUTER WON!");
    }

    resetButton->setEnabled(true); // Disable the button after game reset
    resetButton->setStyleSheet("QPushButton#saveButton {"
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

void XOXApp::resetTable() {
    gameStatusLabel->setText("Game started");
    gameWidget->resetGame();
    resetButton->setEnabled(false); // Disable the button after game reset
    resetButton->setStyleSheet("QPushButton#saveButton {"
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

void XOXApp::drawMove(){
    emit drawButtonClicked();
}

//Freeze the app
void XOXApp::robotDrawingSignal(const bool status){
    if(status){
        this->setEnabled(false);
        toggleOverlay(true);
        gameStatusLabel->setText("Tic-tac-toe cannot be played while the robot is drawing.");
    }
    else{
        this->setEnabled(true);
        toggleOverlay(false);
        gameStatusLabel->setText("Game Started");
    }
}


void XOXApp::toggleOverlay(bool showOverlay) {
    if(showOverlay){
        resetButton->setStyleSheet("QPushButton#saveButton {"
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
        resetButton->setStyleSheet("QPushButton#saveButton {"
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
