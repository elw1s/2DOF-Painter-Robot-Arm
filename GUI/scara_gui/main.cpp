#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStackedWidget>
#include <QFrame>

#include "DrawingApp.h"
#include "ImageUploader.h"
#include "ImagePathsConfig.h"
#include "RobotMainMenu.h"
#include "Settings.h"

void setButtonStyle(QToolButton& button, bool isSelected,const QIcon& icon) {
    QPalette palette = button.palette();

    if (isSelected) {
        button.setStyleSheet("QToolButton {"
                             "    background-color: #19749B;"
                             "    border: 1px solid #33C2FF;"
                             "    color: #DEDEDE;"
                             "    font-family: Abel;"
                             "}"
                             "QToolButton::menu-indicator {"
                             "    image: none;"
                             "}"
    );
    } else {
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
    button.setIcon(icon);
}
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create instances of DrawingApp and ImageUploader
    DrawingApp drawingApp;
    ImageUploader imageUploader;
    //RobotProjectionWidget robotProjectionWidget;
    RobotMainMenu  robotMainMenu;


    // Create a QStackedWidget for the content on the right
    QStackedWidget stackedWidget;
    stackedWidget.addWidget(&robotMainMenu);
    stackedWidget.addWidget(&drawingApp);
    stackedWidget.addWidget(&imageUploader);


    // Create buttons for DrawingApp and ImageUploader
    QToolButton drawingAppButton;
    //drawingAppButton.setIcon(QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
    drawingAppButton.setText("Drawing App");
    drawingAppButton.setIconSize(QSize(30, 30)); // Adjust the icon size
    drawingAppButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(drawingAppButton, false,QIcon(QString::fromStdString(DRAW_IMAGE_TAB))); // Set initial style for the selected button

    QToolButton imageUploaderButton;
    imageUploaderButton.setIcon(QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
    imageUploaderButton.setText("Image Uploader");
    imageUploaderButton.setIconSize(QSize(30, 30)); // Adjust the icon size
    imageUploaderButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(imageUploaderButton, false,QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB))); // Set initial style for the unselected button

    QToolButton robotProjectionButton;
    robotProjectionButton.setIcon(QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    robotProjectionButton.setText("Robot Projection");
    robotProjectionButton.setIconSize(QSize(30, 30)); // Adjust the icon size
    robotProjectionButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(robotProjectionButton, true,QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB_SELECTED))); // Set initial style for the unselected button

    QToolButton playXOXButton;
    playXOXButton.setIcon(QIcon(QString::fromStdString(PLAY_XOX_TAB)));
    playXOXButton.setText("Play XOX");
    playXOXButton.setIconSize(QSize(30, 30)); // Adjust the icon size
    playXOXButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(playXOXButton, false,QIcon(QString::fromStdString(PLAY_XOX_TAB))); // Set initial style for the unselected button

    QToolButton playSudokuButton;
    playSudokuButton.setIcon(QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
    playSudokuButton.setText("Play Sudoku");
    playSudokuButton.setIconSize(QSize(30, 30)); // Adjust the icon size
    playSudokuButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(playSudokuButton, false,QIcon(QString::fromStdString(PLAY_SUDOKU_TAB))); // Set initial style for the unselected button

    QToolButton settingsButton;
    settingsButton.setIcon(QIcon(QString::fromStdString(SETTINGS))); // Replace with your icon path
    settingsButton.setText("Settings");
    settingsButton.setIconSize(QSize(30, 30));
    settingsButton.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(settingsButton, false, QIcon(QString::fromStdString(SETTINGS))); // Set initial style for the unselected button

    QObject::connect(&drawingAppButton, &QToolButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(1);
        setButtonStyle(drawingAppButton, true, QIcon(QString::fromStdString(DRAW_IMAGE_TAB_SELECTED)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
    });

    QObject::connect(&imageUploaderButton, &QToolButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(2);
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, true,  QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB_SELECTED)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false,QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
    });

    QObject::connect(&robotProjectionButton, &QToolButton::clicked, [&]() {
        // Change the index and button styles accordingly
        stackedWidget.setCurrentIndex(0); // Change index based on the actual position in stackedWidget
        setButtonStyle(robotProjectionButton, true,QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB_SELECTED)));
        setButtonStyle(playXOXButton, false,QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false,QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(drawingAppButton, false,QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false,QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
    });

    QObject::connect(&playXOXButton, &QToolButton::clicked, [&]() {
        // Change the index and button styles accordingly
        stackedWidget.setCurrentIndex(0); // Change index based on the actual position in stackedWidget
        setButtonStyle(robotProjectionButton, false,QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
        setButtonStyle(playXOXButton, true,QIcon(QString::fromStdString(PLAY_XOX_TAB_SELECTED)));
        setButtonStyle(playSudokuButton, false,QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(drawingAppButton, false,QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false,QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
    });

    QObject::connect(&playSudokuButton, &QToolButton::clicked, [&]() {
        // Change the index and button styles accordingly
        stackedWidget.setCurrentIndex(0); // Change index based on the actual position in stackedWidget
        setButtonStyle(robotProjectionButton, false,QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
        setButtonStyle(playXOXButton, false,QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, true,QIcon(QString::fromStdString(PLAY_SUDOKU_TAB_SELECTED)));
        setButtonStyle(drawingAppButton, false,QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false,QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
    });

    QObject::connect(&settingsButton, &QToolButton::clicked, [&]() {
        // Show the pop-up window here
        Settings settingsWindow(robotMainMenu.getIP(), robotMainMenu.getPort());
        QObject::connect(&settingsWindow, &Settings::settingsUpdated, &robotMainMenu, &RobotMainMenu::setServerInfo);
        QObject::connect(&settingsWindow, &Settings::disconnectSignal, &robotMainMenu, &RobotMainMenu::disconnectFromServer);
        settingsWindow.exec();
    });

    // Create a QWidget to hold the layout
    QWidget mainWidget;
    mainWidget.setStyleSheet("background-color: #1C1C1C;"); // Set main window background color
    QHBoxLayout* mainLayout = new QHBoxLayout(&mainWidget);

    // Left column with drawingAppButton and imageUploaderButton
    QSize largerButtonSize = drawingAppButton.sizeHint().expandedTo(imageUploaderButton.sizeHint());
    drawingAppButton.setMinimumSize(largerButtonSize);
    imageUploaderButton.setMinimumSize(largerButtonSize);
    robotProjectionButton.setMinimumSize(largerButtonSize);
    playXOXButton.setMinimumSize(largerButtonSize);
    playSudokuButton.setMinimumSize(largerButtonSize);
    settingsButton.setMinimumSize(largerButtonSize);


    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addWidget(&robotProjectionButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(&drawingAppButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(&imageUploaderButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(&playXOXButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(&playSudokuButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(&settingsButton, 0, Qt::AlignCenter);

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
