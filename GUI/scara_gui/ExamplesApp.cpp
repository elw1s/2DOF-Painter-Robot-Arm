#include "ExamplesApp.h"

ExamplesApp::ExamplesApp(QWidget *parent) : QWidget(parent) {
    examplesWidget = new ExamplesWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    saveButton = new QPushButton("Draw", this);
    saveButton->setObjectName("saveButton");

    //mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margins for the main layout
    mainLayout->setContentsMargins(25, 75, 75, 20); // Adjust the margins as needed


    examplesWidget->setStyleSheet("background-color: #D9D9D9;"); // Adjust the background color
    mainLayout->addWidget(examplesWidget);

    saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    saveButton->setFixedWidth(200);
    saveButton->setFixedHeight(50);
    saveButton->setStyleSheet("QPushButton#saveButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 0px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#saveButton:hover {"
                              "    background-color: #57D5FF;"
                              "}");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Push the button to the right
    buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout);

    mainLayout->setAlignment(Qt::AlignRight | Qt::AlignTop); // Align the layout to the top-right

    setLayout(mainLayout);
}

//Freeze the app
void ExamplesApp::robotDrawingSignal(const bool status){
    if(status)
        qDebug() << "Robot is drawing...";
    else
        qDebug() << "Robot is NOT drawing...";
}
