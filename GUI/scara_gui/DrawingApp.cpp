#include "DrawingApp.h"


DrawingApp::DrawingApp(QWidget *parent) : QWidget(parent) {
        drawingArea = new DrawingArea(this);

        saveButton = new QPushButton("Save Image", this);
        connect(saveButton, &QPushButton::clicked, this, &DrawingApp::saveImage);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(drawingArea);
        layout->addWidget(saveButton);

        setLayout(layout);
    }

void DrawingApp::saveImage() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "JPEG Image (*.jpg)");

        if (!fileName.isEmpty()) {
            drawingArea->getPixmap().toImage().save(fileName, "JPG");
        }
}
