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
#include "ExamplesApp.h"
#include "XOXApp.h"
#include "SudokuApp.h"
#include "StatsApp.h"

void setButtonStyle(QToolButton* button, bool isSelected,const QIcon& icon) {
    QPalette palette = button->palette();

    if (isSelected) {
        button->setStyleSheet("QToolButton {"
                             "    background-color: #19749B;"
                             "    border: 1px solid #33C2FF;"
                             "    color: #DEDEDE;"
                             "    font-family: Abel;"
                             "    font-size: 11;"
                             "}"
                             "QToolButton::menu-indicator {"
                             "    image: none;"
                             "}"
    );
    } else {
        button->setStyleSheet("QToolButton {"
                             "    background-color: #4F4F4F;"
                             "    border: 1px solid #767676;"
                             "    color: #DEDEDE;"
                             "    font-family: Abel;"
                             "    font-size: 11;"
                             "}"
                             "QToolButton::menu-indicator {"
                             "    image: none;"
                             "}");
    }
    button->setIcon(icon);
}
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create instances of DrawingApp and ImageUploader
    DrawingApp drawingApp = new DrawingApp();
    ImageUploader imageUploader = new ImageUploader();
    ExamplesApp examplesApp = new ExamplesApp();
    //RobotProjectionWidget robotProjectionWidget;
    RobotMainMenu  robotMainMenu = new RobotMainMenu();
    XOXApp xoxApp = new XOXApp();
    SudokuApp sudoku = new SudokuApp();
    StatsApp stats = new StatsApp();

    QDir dir;
    QString folderPath = dir.temp().path() + "/cse396";
    qDebug() <<"Folder path ==> " << folderPath;

    // Always attempt to create the directory using mkpath()
    if (!dir.mkpath(folderPath)) {
        qDebug("Error creating temporary folder");
        return -1;
    }



    // Create a QStackedWidget for the content on the right
    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(&robotMainMenu);
    stackedWidget->addWidget(&examplesApp);
    stackedWidget->addWidget(&drawingApp);
    stackedWidget->addWidget(&imageUploader);
    stackedWidget->addWidget(&xoxApp);
    stackedWidget->addWidget(&sudoku);
    stackedWidget->addWidget(&stats);

    // Create buttons for DrawingApp and ImageUploader
    QToolButton *drawingAppButton = new QToolButton();
    //drawingAppButton.setIcon(QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
    drawingAppButton->setText("Drawing App");
    drawingAppButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    drawingAppButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(drawingAppButton, false,QIcon(QString::fromStdString(DRAW_IMAGE_TAB))); // Set initial style for the selected button

    QToolButton *imageUploaderButton = new QToolButton();
    imageUploaderButton->setIcon(QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
    imageUploaderButton->setText("Image Uploader");
    imageUploaderButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    imageUploaderButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(imageUploaderButton, false,QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB))); // Set initial style for the unselected button

    QToolButton *robotProjectionButton = new QToolButton();
    robotProjectionButton->setIcon(QIcon(QString::fromStdString(HOME_PAGE)));
    robotProjectionButton->setText("Main Page");
    robotProjectionButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    robotProjectionButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(robotProjectionButton, true,QIcon(QString::fromStdString(HOME_PAGE_SELECTED))); // Set initial style for the unselected button

    QToolButton *playXOXButton = new QToolButton();
    playXOXButton->setIcon(QIcon(QString::fromStdString(PLAY_XOX_TAB)));
    playXOXButton->setText("Play XOX");
    playXOXButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    playXOXButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(playXOXButton, false,QIcon(QString::fromStdString(PLAY_XOX_TAB))); // Set initial style for the unselected button

    QToolButton *playSudokuButton = new QToolButton();
    playSudokuButton->setIcon(QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
    playSudokuButton->setText("Play Sudoku");
    playSudokuButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    playSudokuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(playSudokuButton, false,QIcon(QString::fromStdString(PLAY_SUDOKU_TAB))); // Set initial style for the unselected button

    QToolButton *settingsButton = new QToolButton();
    settingsButton->setIcon(QIcon(QString::fromStdString(SETTINGS))); // Replace with your icon path
    settingsButton->setText("Settings");
    settingsButton->setIconSize(QSize(30, 30));
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(settingsButton, false, QIcon(QString::fromStdString(SETTINGS))); // Set initial style for the unselected button

    QToolButton *examplesButton = new QToolButton();
    examplesButton->setIcon(QIcon(QString::fromStdString(EXAMPLES)));
    examplesButton->setText("Examples");
    examplesButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    examplesButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(examplesButton, false,QIcon(QString::fromStdString(EXAMPLES))); // Set initial style for the unselected button

    QToolButton *statsButton = new QToolButton();
    statsButton->setIcon(QIcon(QString::fromStdString(STATS)));
    statsButton->setText("Stats");
    statsButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    statsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(statsButton, false,QIcon(QString::fromStdString(STATS))); // Set initial style for the unselected button

    QToolButton *openGazeboButton = new QToolButton();
    openGazeboButton->setIcon(QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    openGazeboButton->setText("Open Gazebo");
    openGazeboButton->setIconSize(QSize(30, 30)); // Adjust the icon size
    openGazeboButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setButtonStyle(openGazeboButton, false,QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB))); // Set initial style for the unselected button

    QObject::connect(drawingAppButton, &QToolButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(2);
        setButtonStyle(drawingAppButton, true, QIcon(QString::fromStdString(DRAW_IMAGE_TAB_SELECTED)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(HOME_PAGE)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(examplesButton, false, QIcon(QString::fromStdString(EXAMPLES)));
        setButtonStyle(statsButton, false, QIcon(QString::fromStdString(STATS)));
        setButtonStyle(openGazeboButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    });

    QObject::connect(imageUploaderButton, &QToolButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(3);
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, true, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB_SELECTED)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(HOME_PAGE)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(examplesButton, false, QIcon(QString::fromStdString(EXAMPLES)));
        setButtonStyle(statsButton, false, QIcon(QString::fromStdString(STATS)));
        setButtonStyle(openGazeboButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    });

    QObject::connect(robotProjectionButton, &QToolButton::clicked, [&]() {
        // Change the index and button styles accordingly
        stackedWidget->setCurrentIndex(0); // Change index based on the actual position in stackedWidget
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, true, QIcon(QString::fromStdString(HOME_PAGE_SELECTED)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(examplesButton, false, QIcon(QString::fromStdString(EXAMPLES)));
        setButtonStyle(statsButton, false, QIcon(QString::fromStdString(STATS)));
        setButtonStyle(openGazeboButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    });

    QObject::connect(playXOXButton, &QToolButton::clicked, [&]() {
        // Change the index and button styles accordingly
        stackedWidget->setCurrentIndex(4); // Change index based on the actual position in stackedWidget
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(HOME_PAGE)));
        setButtonStyle(playXOXButton, true, QIcon(QString::fromStdString(PLAY_XOX_TAB_SELECTED)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(examplesButton, false, QIcon(QString::fromStdString(EXAMPLES)));
        setButtonStyle(statsButton, false, QIcon(QString::fromStdString(STATS)));
        setButtonStyle(openGazeboButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    });

    QObject::connect(playSudokuButton, &QToolButton::clicked, [&]() {
        // Change the index and button styles accordingly
        stackedWidget->setCurrentIndex(5); // Change index based on the actual position in stackedWidget
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(HOME_PAGE)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, true, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB_SELECTED)));
        setButtonStyle(examplesButton, false, QIcon(QString::fromStdString(EXAMPLES)));
        setButtonStyle(statsButton, false, QIcon(QString::fromStdString(STATS)));
        setButtonStyle(openGazeboButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    });

    QObject::connect(settingsButton, &QToolButton::clicked, [&]() {
        // Show the pop-up window here
        Settings settingsWindow(robotMainMenu.getIP(), robotMainMenu.getPort());
        QObject::connect(&settingsWindow, &Settings::settingsUpdated, &robotMainMenu, &RobotMainMenu::setServerInfo);
        QObject::connect(&settingsWindow, &Settings::disconnectSignal, &robotMainMenu, &RobotMainMenu::disconnectFromServer);
        settingsWindow.exec();
    });

    QObject::connect(examplesButton, &QToolButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(1);
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(HOME_PAGE)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(examplesButton, true, QIcon(QString::fromStdString(EXAMPLES_SELECTED)));
        setButtonStyle(statsButton, false, QIcon(QString::fromStdString(STATS)));
        setButtonStyle(openGazeboButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    });

    QObject::connect(statsButton, &QToolButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(6);
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(HOME_PAGE)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(examplesButton, false, QIcon(QString::fromStdString(EXAMPLES)));
        setButtonStyle(statsButton, true, QIcon(QString::fromStdString(STATS_SELECTED)));
        setButtonStyle(openGazeboButton, false, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB)));
    });

    QObject::connect(openGazeboButton, &QToolButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
        setButtonStyle(drawingAppButton, false, QIcon(QString::fromStdString(DRAW_IMAGE_TAB)));
        setButtonStyle(imageUploaderButton, false, QIcon(QString::fromStdString(UPLOAD_IMAGE_TAB)));
        setButtonStyle(robotProjectionButton, false, QIcon(QString::fromStdString(HOME_PAGE)));
        setButtonStyle(playXOXButton, false, QIcon(QString::fromStdString(PLAY_XOX_TAB)));
        setButtonStyle(playSudokuButton, false, QIcon(QString::fromStdString(PLAY_SUDOKU_TAB)));
        setButtonStyle(examplesButton, false, QIcon(QString::fromStdString(EXAMPLES)));
        setButtonStyle(statsButton, false, QIcon(QString::fromStdString(STATS)));
        setButtonStyle(openGazeboButton, true, QIcon(QString::fromStdString(ROBOT_PROJECTION_TAB_SELECTED)));
    });

    //Connections

    QObject::connect(&drawingApp, &DrawingApp::drawButtonClicked, &robotMainMenu, &RobotMainMenu::drawButtonClicked);
    QObject::connect(&imageUploader, &ImageUploader::drawButtonClicked, &robotMainMenu, &RobotMainMenu::drawButtonClicked);
    QObject::connect(&examplesApp, &ExamplesApp::drawButtonClicked, &robotMainMenu, &RobotMainMenu::drawButtonClicked);
    QObject::connect(&sudoku, &SudokuApp::drawButtonClicked, &robotMainMenu, &RobotMainMenu::drawButtonClicked);
    QObject::connect(&xoxApp, &XOXApp::drawButtonClicked, &robotMainMenu, &RobotMainMenu::drawButtonClicked);

    QObject::connect(&robotMainMenu, &RobotMainMenu::drawingStatus, &drawingApp, &DrawingApp::robotDrawingSignal);
    QObject::connect(&robotMainMenu, &RobotMainMenu::drawingStatus, &imageUploader, &ImageUploader::robotDrawingSignal);
    QObject::connect(&robotMainMenu, &RobotMainMenu::drawingStatus, &examplesApp, &ExamplesApp::robotDrawingSignal);
    QObject::connect(&robotMainMenu, &RobotMainMenu::drawingStatus, &sudoku, &SudokuApp::robotDrawingSignal);
    QObject::connect(&robotMainMenu, &RobotMainMenu::drawingStatus, &xoxApp, &XOXApp::robotDrawingSignal);


    // Create a QWidget to hold the layout
    QWidget *mainWidget = new QWidget;
    mainWidget->setStyleSheet("background-color: #1C1C1C;"); // Set main window background color
    QHBoxLayout* mainLayout = new QHBoxLayout(mainWidget);

    // Left column with drawingAppButton and imageUploaderButton
    QSize largerButtonSize = imageUploaderButton->sizeHint().expandedTo(imageUploaderButton->sizeHint());
    drawingAppButton->setMinimumSize(largerButtonSize);
    imageUploaderButton->setMinimumSize(largerButtonSize);
    robotProjectionButton->setMinimumSize(largerButtonSize);
    playXOXButton->setMinimumSize(largerButtonSize);
    playSudokuButton->setMinimumSize(largerButtonSize);
    settingsButton->setMinimumSize(largerButtonSize);
    examplesButton->setMinimumSize(largerButtonSize);
    statsButton->setMinimumSize(largerButtonSize);
    openGazeboButton->setMinimumSize(largerButtonSize);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addWidget(robotProjectionButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(statsButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(examplesButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(drawingAppButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(imageUploaderButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(playXOXButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(playSudokuButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(openGazeboButton, 0, Qt::AlignCenter);
    leftLayout->addWidget(settingsButton, 0, Qt::AlignCenter);

    leftLayout->setSpacing(5); // Adjust the spacing between buttons
    leftLayout->setContentsMargins(0, 100, 0, 100); // Set top and bottom margins to 20
    leftLayout->addStretch(1);


    QWidget* leftWidget = new QWidget;
    leftWidget->setLayout(leftLayout);
    leftWidget->setStyleSheet("background-color: #212121; margin: 5 0 5 5;"); // Set left column background color
    // Right column with stackedWidget
    QWidget rightColumn;
    QVBoxLayout* rightLayout = new QVBoxLayout(&rightColumn);
    rightLayout->addWidget(stackedWidget);
    rightColumn.setStyleSheet("background-color: #1C1C1C;"); // Set right column background color

    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(&rightColumn);
    mainLayout->setSpacing(0); // Remove spacing between columns


    mainWidget->resize(1024, 768);
    mainWidget->show();
    return app.exec();
}
