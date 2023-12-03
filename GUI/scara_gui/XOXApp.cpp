#include "XOXApp.h"

XOXApp::XOXApp(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this); // Create a QVBoxLayout for XOXApp

    gameStatusLabel = new QLabel("Game started", this); // Create the gameStatusLabel
    gameStatusLabel->setAlignment(Qt::AlignHCenter);
    gameStatusLabel->setStyleSheet("color: white;"); // Set text color to white
    gameStatusLabel->setFont(QFont("Arial", 30)); // Set font size to 30
    layout->addWidget(gameStatusLabel); // Add gameStatusLabel to the layout

    gameWidget = new XOXWidget(this);
    connect(gameWidget, &XOXWidget::gameOver, this, &XOXApp::gameFinished);
    layout->addWidget(gameWidget); // Add gameWidget to the layout

    resetButton = new QPushButton("Play Again", this);
    layout->addWidget(resetButton,Qt::AlignHCenter); // Add resetButton to the layout
    resetButton->setEnabled(false); // Initially disable the button
    resetButton->setStyleSheet("background-color: grey;"); // Set default grey color
    resetButton->setMaximumWidth(250);
    resetButton->setMaximumHeight(50);
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
    resetButton->setStyleSheet("background-color: green;"); // Set button color to grey
}

void XOXApp::resetTable() {
    gameStatusLabel->setText("Game started");
    gameWidget->resetGame();
    resetButton->setEnabled(false); // Disable the button after game reset
    resetButton->setStyleSheet("background-color: grey;"); // Set button color to grey

}
