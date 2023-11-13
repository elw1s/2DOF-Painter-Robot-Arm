#include <QApplication>
#include <QTabWidget>
#include "DrawingApp.h"
#include "ImageUploader.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create a QTabWidget to hold the DrawingApp and ImageUploader
    QTabWidget tabWidget;

    // Create instances of DrawingApp and ImageUploader
    DrawingApp drawingApp;
    ImageUploader imageUploader;

    // Add tabs for each program
    tabWidget.addTab(&drawingApp, "Drawing App");
    tabWidget.addTab(&imageUploader, "Image Uploader");

    // Set the main window properties
    tabWidget.setGeometry(100, 100, 800, 600);
    tabWidget.setWindowTitle("Qt Programs");

    // Show the main window
    tabWidget.show();

    return app.exec();
}
