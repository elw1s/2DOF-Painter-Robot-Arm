#include "DrawingApp.h"


DrawingApp::DrawingApp(QWidget *parent) : QWidget(parent) {
    drawingArea = new DrawingArea(this);

    saveButton = new QPushButton("Draw", this);
    saveButton->setObjectName("saveButton"); // Set object name to apply specific styles
    connect(saveButton, &QPushButton::clicked, this, &DrawingApp::saveImage);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 75, 75, 20); // Adjust the margins as needed

    drawingArea->setStyleSheet("background-color: #D9D9D9; margin: 75px 75px 20px 25px;");

    layout->addWidget(drawingArea);

    // Customize Save Image button
    saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    saveButton->setFixedWidth(250);
    saveButton->setFixedHeight(50); // Set the fixed height
    saveButton->setStyleSheet("QPushButton#saveButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 75px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#saveButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Push the button to the right
    buttonLayout->addWidget(saveButton);
    layout->addLayout(buttonLayout);

    penStrokeDropdown = new QComboBox(this);
    penStrokeDropdown->addItem("1");
    penStrokeDropdown->addItem("2");
    penStrokeDropdown->addItem("3");
    penStrokeDropdown->addItem("4");
    penStrokeDropdown->setCurrentIndex(0); // Set default stroke to 1

    connect(penStrokeDropdown, QOverload<int>::of(&QComboBox::activated), this, &DrawingApp::setPenStroke);
    layout->addWidget(penStrokeDropdown);

    layout->setAlignment(Qt::AlignRight | Qt::AlignTop); // Align the layout to the top-right

    setLayout(layout);
}
void DrawingApp::saveImage() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "JPEG Image (*.jpg)");

        if (!fileName.isEmpty()) {
            drawingArea->getPixmap().toImage().save(fileName, "JPG");
        }
}

void DrawingApp::setPenStroke(int stroke) {
        // Assuming DrawingArea has a function to set the pen stroke
        drawingArea->setPenStroke(stroke + 1); // +1 to align with the dropdown (index 0-3) and stroke values (1-4)
}
