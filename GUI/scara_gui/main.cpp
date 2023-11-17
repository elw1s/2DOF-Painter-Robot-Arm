#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStackedWidget>
#include <QFrame>

#include "DrawingApp.h"
#include "ImageUploader.h"
#include "ImagePathsConfig.h"

void setButtonStyle(QToolButton& button, bool isSelected) {
    QPalette palette = button.palette();

    if (isSelected) {
        //palette.setColor(QPalette::Button, QColor(51, 194, 255, 77)); // #33C2FF with 30% opacity
        //palette.setColor(QPalette::ButtonText, QColor(222, 222, 222)); // Text color when selected
        button.setStyleSheet("QToolButton {"
                             "    background-color: rgba(51, 194, 255, 77);"
                             "    border: 1px solid #33C2FF;"
                             "    color: #DEDEDE;"
                             "    font-family: Abel;"
                             "}"
                             "QToolButton::menu-indicator {"
                             "    image: none;"
                             "}"
    );
    } else {
        //palette.setColor(QPalette::Button, QColor(79, 79, 79, 1)); // Background color when not selected
        //palette.setColor(QPalette::ButtonText, QColor(222, 222, 222)); // Text color when not selected
        button.setStyleSheet("QToolButton {"
                             "    background-color: #4F4F4F;"
                             "    border: 1px solid #767676;"
                             "    color: #DEDEDE;"
                             "    font-family: Abel;"
                             "}"
                             "QToolButton::menu-indicator {"
                             "    image: none;"
                             "}");
    }

    //button.setPalette(palette);
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
                          "QToolButton::menu-indicator {"
                          "    image: none;"
                          "}";

    QString drawingAppButtonStyle = buttonStyle.arg("#4F4F4F").arg("#767676").arg("#DEDEDE");
    QString imageUploaderButtonStyle = buttonStyle.arg("#4F4F4F").arg("#767676").arg("#DEDEDE");

    // Set the icon background color to match the button background color
    //drawingAppButtonStyle += "QToolButton::icon { background-color: #4F4F4F; }";
    //imageUploaderButtonStyle += "QToolButton::icon { background-color: #4F4F4F; }";
    drawingAppButton.setStyleSheet(drawingAppButtonStyle);
    imageUploaderButton.setStyleSheet(imageUploaderButtonStyle);

    // Create a QWidget to hold the layout
    QWidget mainWidget;
    mainWidget.setStyleSheet("background-color: #1C1C1C;"); // Set main window background color
    QHBoxLayout* mainLayout = new QHBoxLayout(&mainWidget);

    // Left column with drawingAppButton and imageUploaderButton
    QSize largerButtonSize = drawingAppButton.sizeHint().expandedTo(imageUploaderButton.sizeHint());
    drawingAppButton.setMinimumSize(largerButtonSize);
    imageUploaderButton.setMinimumSize(largerButtonSize);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addWidget(&drawingAppButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(&imageUploaderButton, 0, Qt::AlignCenter);
    leftLayout->setSpacing(5); // Adjust the spacing between buttons
    leftLayout->setContentsMargins(0, 100, 0, 100); // Set top and bottom margins to 20
    leftLayout->addStretch(1);


    QWidget* leftWidget = new QWidget;
    leftWidget->setLayout(leftLayout);
    leftWidget->setStyleSheet("background-color: #212121; margin: 5 0 5 5;"); // Set left column background color
    // Right column with stackedWidget
    QWidget rightColumn;
    QVBoxLayout* rightLayout = new QVBoxLayout(&rightColumn);
    rightLayout->addWidget(&stackedWidget);
    rightColumn.setStyleSheet("background-color: #1C1C1C;"); // Set right column background color

    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(&rightColumn);
    mainLayout->setSpacing(0); // Remove spacing between columns

    mainWidget.resize(1024, 768);
    mainWidget.show();
    return app.exec();
}
