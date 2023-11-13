#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStackedWidget>

#include "DrawingApp.h"
#include "ImageUploader.h"
#include "ImagePathsConfig.h"

void setButtonStyle(QToolButton& button, bool isSelected) {
    QPalette palette = button.palette();

    if (isSelected) {
        palette.setColor(QPalette::Button, QColor(0, 77, 110, 88)); // Background color when selected
        palette.setColor(QPalette::ButtonText, QColor(222, 222, 222)); // Text color when selected
    } else {
        palette.setColor(QPalette::Button, QColor(79, 79, 79, 1)); // Background color
        palette.setColor(QPalette::ButtonText, QColor(222, 222, 222)); // Text color
    }

    button.setPalette(palette);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create instances of DrawingApp and ImageUploader
    DrawingApp drawingApp;
    ImageUploader imageUploader;

    // Create a QStackedWidget for the content on the right
    QStackedWidget stackedWidget;
    stackedWidget.addWidget(&drawingApp);
    stackedWidget.addWidget(&imageUploader);

    // Create buttons for DrawingApp and ImageUploader
    QToolButton drawingAppButton;
    drawingAppButton.setIcon(QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
    drawingAppButton.setText("Drawing App");
    drawingAppButton.setIconSize(QSize(64, 64)); // Adjust the icon size
    drawingAppButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(drawingAppButton, true); // Set initial style for the selected button

    QToolButton imageUploaderButton;
    imageUploaderButton.setIcon(QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
    imageUploaderButton.setText("Image Uploader");
    imageUploaderButton.setIconSize(QSize(64, 64)); // Adjust the icon size
    imageUploaderButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(imageUploaderButton, false); // Set initial style for the unselected button

    // Connect buttons to switch the stackedWidget index and change style
    QObject::connect(&drawingAppButton, &QToolButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(0);
        setButtonStyle(drawingAppButton, true);
        setButtonStyle(imageUploaderButton, false);
    });

    QObject::connect(&imageUploaderButton, &QToolButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(1);
        setButtonStyle(drawingAppButton, false);
        setButtonStyle(imageUploaderButton, true);
    });

    // Apply styles to buttons including the icon background
    QString buttonStyle = "QToolButton {"
                          "    background-color: %1;"
                          "    border: 1px solid %2;"
                          "    color: %3;"
                          "    font-family: Abel;"
                          "}"

                          "QToolButton:checked {"
                          "    background-color: rgba(51, 194, 255, 30%);"
                          "    border: 1px solid rgba(51, 194, 255, 100%);"
                          "}";

    QString drawingAppButtonStyle = buttonStyle.arg("#4F4F4F").arg("#767676").arg("#DEDEDE");
    QString imageUploaderButtonStyle = buttonStyle.arg("#4F4F4F").arg("#767676").arg("#DEDEDE");

    drawingAppButton.setStyleSheet(drawingAppButtonStyle);
    imageUploaderButton.setStyleSheet(imageUploaderButtonStyle);

    // Create a QVBoxLayout for the buttons on the left
    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addWidget(&drawingAppButton);
    leftLayout->addWidget(&imageUploaderButton);
    leftLayout->setSpacing(2); // Adjust the spacing between buttons

    // Create an HBoxLayout to arrange buttons and stackedWidget
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(&stackedWidget);

    // Create a QWidget to hold the layout
    QWidget mainWidget;
    mainWidget.setLayout(mainLayout);

    // Set the mainWidget as the central widget and set the initial size
    mainWidget.resize(800, 600);
    mainWidget.show();

    return app.exec();
}
