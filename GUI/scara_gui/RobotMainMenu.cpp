#include "RobotMainMenu.h"

RobotMainMenu::RobotMainMenu(QWidget *parent) : QWidget(parent) {
    //timeData = new QVector();
    //sensorData = new QVector();

    label = new QLabel("Preview of drawing");
    projectionWidget = new RobotProjectionWidget(this);
    bottomButton = new QPushButton("View 3D Virtualization");
    bottomButton->setObjectName("bottomButton");
    bottomButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bottomButton->setFixedWidth(200);
    bottomButton->setFixedHeight(50); // Set the fixed height
    bottomButton->setStyleSheet("QPushButton#bottomButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 0px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#bottomButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");


    projectionWidget->setStyleSheet("background-color: #D9D9D9; border: 5px solid red;");

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(projectionWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    loadingLabelLayout = new QHBoxLayout;
    textLabel = new QLabel();
    textLabel->setText("Device is not connected!");
    loadingLabel = new QLabel(this);
    loadingMovie = new QMovie(QString::fromStdString(LOADING_GIF)); // Provide the path to your loading GIF
    loadingMovie->setScaledSize(QSize(20, 20)); // Set the size of the movie (GIF)
    loadingLabel->setMovie(loadingMovie);
    loadingLabel->setVisible(false);

    loadingProgressBar = new QProgressBar(this);
    loadingProgressBar->setTextVisible(true);
    loadingProgressBar->setMinimum(0);
    loadingProgressBar->setMaximum(100);
    loadingProgressBar->setVisible(false); // Initially hidden

    loadingLabelLayout->addWidget(loadingLabel);
    loadingLabelLayout->addWidget(textLabel);
    loadingLabelLayout->setAlignment(Qt::AlignLeft);

    // Add loading elements below the projection widget
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(label,10,Qt::AlignCenter);
    leftLayout->addWidget(scrollArea,90);
    //leftLayout->addWidget(loadingLabel); // Add the loading circle widget
    leftLayout->addLayout(loadingLabelLayout);
    leftLayout->addWidget(loadingProgressBar); // Add the progress bar widget

    QVBoxLayout *rightTopLayout = new QVBoxLayout;
    sensorPlot  = new QCustomPlot();
    sensorPlot->addGraph();
    sensorPlot->yAxis->setLabel("Sensor Value");
    sensorPlot->yAxis->setRange(-120, 120); // Modify the range as needed
    sensorPlot->setBackground(QBrush(QColor("#1C1C1C")));
    sensorPlot->xAxis->setTickLabelColor(Qt::white);
    sensorPlot->yAxis->setTickLabelColor(Qt::white);
    sensorPlot->xAxis->setLabelColor(Qt::white);
    sensorPlot->yAxis->setLabelColor(Qt::white);
    sensorPlot->xAxis->setLabel("Time"); // Set label with current time
    QPen graphPen;
    graphPen.setColor(Qt::red); // Set the line color to red
    graphPen.setWidth(2);
    sensorPlot->graph(0)->setPen(graphPen); // Adjust the width (2 indicates the thickness)
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("hh:mm:ss"); // Set tick label format
    dateTimeTicker->setTickCount(3); // Show only four tickss
    dateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    sensorPlot->xAxis->setTicker(dateTimeTicker); // Set ticker

    QLabel *currentTimeLabel = new QLabel();
    currentTimeLabel->setStyleSheet("color: white; font-size: 12px;"); // Style for time display
    rightTopLayout->addWidget(currentTimeLabel, 0 ,Qt::AlignHCenter); // Add the label to the layout

    // Create a QTimer to update the time label every second
    QTimer *timeUpdateTimer = new QTimer(this);
    connect(timeUpdateTimer, &QTimer::timeout, [=]() {
        QDateTime currentTime = QDateTime::currentDateTime();
        QString formattedTime = currentTime.toString("hh:mm:ss");
        currentTimeLabel->setText(formattedTime);
    });
    timeUpdateTimer->start(1000); // Start the timer with a 1-second interval
    rightTopLayout->addWidget(sensorPlot);

    // Increase width and height of sensorPlot (QCustomPlot)
    sensorPlot->setMinimumSize(150, 130); // Set the minimum size for the QCustomPlot
    sensorPlot->setMaximumSize(400,3000);
    // Modify size policies to control how the widgets expand in the layout
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    projectionWidget->setSizePolicy(sizePolicy);
    sensorPlot->setSizePolicy(sizePolicy);

    sensorPlot->xAxis->setTickLabels(false);
    sensorPlot->xAxis->setTickPen(QPen(QColor("#33C2FF"))); // Major ticks on x-axis
    sensorPlot->xAxis->setSubTickPen(QPen(QColor("#19749B"))); // Minor ticks on x-axis
    sensorPlot->yAxis->setTickPen(QPen(QColor("#33C2FF"))); // Major ticks on y-axis
    sensorPlot->yAxis->setSubTickPen(QPen(QColor("#19749B"))); // Minor ticks on y-axis

    QVBoxLayout *rightMiddleLayout = new QVBoxLayout;
    waveformPlot = new QCustomPlot(this);
    waveformPlot->setBackground(QBrush(QColor("#1C1C1C")));
    waveformPlot->xAxis->setTickLabelColor(Qt::white);
    waveformPlot->yAxis->setTickLabelColor(Qt::white);
    waveformPlot->xAxis->setLabelColor(Qt::white);
    waveformPlot->yAxis->setLabelColor(Qt::white);
    waveformPlot->yAxis->setRange(-210, 210); // Modify the range as needed
    waveformPlot->xAxis->setLabel("Time"); // Set label with current time
    waveformPlot->yAxis->setLabel("Servo Angles");
    waveformPlot->setMinimumSize(150, 130); // Set the minimum size for the QCustomPlot
    waveformPlot->setMaximumSize(400,300);
    waveformPlot->setSizePolicy(sizePolicy);
    waveformPlot->xAxis->setTicker(dateTimeTicker); // Set ticker


    waveformPlot->xAxis->setTickLabels(false);

    // Add graphs for each line
    waveformPlot->addGraph();
    waveformPlot->graph(0)->setPen(QPen(QColor("#ed4046"), 2)); // Red line

    waveformPlot->addGraph();
    waveformPlot->graph(1)->setPen(QPen(QColor("#327ba8"), 2)); // Blue line with color #327ba8

    waveformPlot->addGraph();
    waveformPlot->graph(2)->setPen(QPen(QColor("#1bb310"), 2)); // Green line

    waveformPlot->xAxis->setTickPen(QPen(QColor("#33C2FF"))); // Major ticks on x-axis
    waveformPlot->xAxis->setSubTickPen(QPen(QColor("#19749B"))); // Minor ticks on x-axis
    waveformPlot->yAxis->setTickPen(QPen(QColor("#33C2FF"))); // Major ticks on y-axis
    waveformPlot->yAxis->setSubTickPen(QPen(QColor("#19749B"))); // Minor ticks on y-axis

    // Set up the layout for the waveform plot and its label
    QVBoxLayout *waveformLayout = new QVBoxLayout;
    waveformLayout->addWidget(waveformPlot); // Add the waveform plot to the layout

    // Add label for color and value key
    QHBoxLayout *legendLabelLayout = new QHBoxLayout;
    redLabel = new QLabel();
    redLabel->setText("Shoulder Motor: 0");
    blueLabel = new QLabel();
    blueLabel->setText("Elbow Motor: 0");
    greenLabel = new QLabel();
    greenLabel->setText("Lifting Motor: 0");

    redLabel->setStyleSheet("color: #ed4046;"); // Style the label as needed
    blueLabel->setStyleSheet("color: #327ba8;"); // Style the label as needed
    greenLabel->setStyleSheet("color: #1bb310;"); // Style the label as needed
    legendLabelLayout->addWidget(redLabel);
    legendLabelLayout->addWidget(blueLabel);
    legendLabelLayout->addWidget(greenLabel);
    legendLabelLayout->setAlignment(Qt::AlignCenter); // Center align the labels within the layout
    waveformLayout->addLayout(legendLabelLayout);
    rightMiddleLayout->addLayout(waveformLayout);


    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(rightTopLayout);
    rightLayout->addLayout(rightMiddleLayout);
    rightLayout->addStretch(); // Add a stretch before the bottom button
    rightLayout->addWidget(bottomButton);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    updateTimerServo = new QTimer(this);
    connect(updateTimerServo, &QTimer::timeout, this, &RobotMainMenu::onUpdateTimerServo);
    updateTimerSensor = new QTimer(this);
    connect(updateTimerSensor, &QTimer::timeout, this, &RobotMainMenu::onUpdateTimerSensor);

    ipAddress = QString();
    port = 0;
    initializeServerListener();
    }

void RobotMainMenu::onUpdateTimerServo() {
    if (serverListenerThread && serverListenerThread->isRunning()){
        if(this->firstAngleData.size() != 0 && this->secondAngleData.size() != 0 && this->thirdAngleData.size() != 0){
            updateServoAngleGraph(this->firstAngleData.back(), this->secondAngleData.back(), this->thirdAngleData.back());
            updateTimerServo->start(1000);
        }
    }
}

void RobotMainMenu::onUpdateTimerSensor() {
    if (serverListenerThread && serverListenerThread->isRunning()){
        if(this->sensorData.size() != 0){
            updateSensorGraph(this->sensorData.back());
            updateTimerSensor->start(1000);
        }
    }
}

void RobotMainMenu::updateSensorGraph(double sensorValue) {
    qDebug()  << "inside uupdateSensorGraph";
    sensorPlot->xAxis->setTickLabels(true);
    //Add the new data point to the vectors
    QDateTime currentDateTime = QDateTime::currentDateTime(); // Get current date and time
    timeDataSensor.append(currentDateTime.toMSecsSinceEpoch() / 1000.0); // Convert QDateTime to seconds since epoch
    sensorData.append(sensorValue);

    if(timeDataSensor.size() ==  4){
        timeDataSensor.removeFirst();
    }

    if(sensorData.size() ==  4){
        sensorData.removeFirst();
    }

    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("hh:mm:ss"); // Set tick label format
    dateTimeTicker->setTickCount(3); // Show only four ticks
    dateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    sensorPlot->xAxis->setTicker(dateTimeTicker); // Set ticker


    // Update the graph with the new data
    sensorPlot->graph(0)->setData(timeDataSensor, sensorData);
    sensorPlot->xAxis->setRange(timeDataSensor.first(), timeDataSensor.last()); // Set range based on timeDate
    sensorPlot->replot(); // Redraw the plot
}

void RobotMainMenu::updateServoAngleGraph(int firstAngle, int secondAngle, int thirdAngle){
    waveformPlot->xAxis->setTickLabels(true);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    timeDataServo.append(currentDateTime.toMSecsSinceEpoch() / 1000.0);
    qDebug()  << "inside updateServoAnglegraph";
    if(timeDataServo.size() == 4)
    {
        timeDataServo.removeFirst();
    }
    firstAngleData.append(firstAngle);
    secondAngleData.append(secondAngle);
    thirdAngleData.append(thirdAngle);

    if(firstAngleData.size() == 4)
    {
        firstAngleData.removeFirst();
    }
    if(secondAngleData.size() == 4)
    {
        secondAngleData.removeFirst();
    }
    if(thirdAngleData.size() == 4)
    {
        thirdAngleData.removeFirst();
    }

    QString redLabelText = QString("Shoulder Motor: %1").arg(firstAngle);
    QString blueLabelText = QString("Elbow Motor: %1").arg(secondAngle);
    QString greenLabelText = QString("Lifting Motor: %1").arg(thirdAngle);

    redLabel->setText(redLabelText);
    blueLabel->setText(blueLabelText);
    greenLabel->setText(greenLabelText);


    // Update each graph with its respective data
    //if(updateTimer->remainingTime() <= 0){
        waveformPlot->graph(0)->setData(timeDataServo, firstAngleData); // Green line
        waveformPlot->graph(1)->setData(timeDataServo, secondAngleData); // Red line
        waveformPlot->graph(2)->setData(timeDataServo, thirdAngleData); // Blue line

        QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
        dateTimeTicker->setDateTimeFormat("hh:mm:ss"); // Set tick label format
        dateTimeTicker->setTickCount(3); // Show only four ticks
        dateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
        waveformPlot->xAxis->setTicker(dateTimeTicker); // Set ticker

        // Set range based on the stored timeDataServo
        if (!timeDataServo.isEmpty()) {
        waveformPlot->xAxis->setRange(timeDataServo.first(), timeDataServo.last());
        }

        waveformPlot->replot();
    //}
}


void RobotMainMenu::setServerInfo(const QString& ip, int port) {
    ipAddress = ip;
    this->port = port;
    initializeServerListener();
}

void RobotMainMenu::initializeServerListener() {
    // Check if ipAddress and port are set before initializing the listener
    if (!ipAddress.isEmpty() && port != 0) {
        serverListenerThread = new ServerListenerThread(ipAddress, port, this);
        connect(serverListenerThread, &ServerListenerThread::linesReceived,
                projectionWidget, &RobotProjectionWidget::loadLinesFromJson);
        connect(serverListenerThread, &ServerListenerThread::allLinesReceived,
                projectionWidget, &RobotProjectionWidget::setAllLines);
        connect(serverListenerThread, &ServerListenerThread::totalLineNumber, this, &RobotMainMenu::setTotalLineNumber);
        connect(serverListenerThread, &ServerListenerThread::loadingProgress, this, &RobotMainMenu::showLoadingBar);
        connect(serverListenerThread, &ServerListenerThread::sensorValues,
                this, &RobotMainMenu::updateSensorGraph);
        connect(serverListenerThread, &ServerListenerThread::servoAngles,
                this, &RobotMainMenu::updateServoAngleGraph);
        serverListenerThread->start();
        updateTimerSensor->start(1000); // 1000ms = 1 second interval for updates
        updateTimerServo->start(1000); // 1000ms = 1 second interval for updates

    } else {
        qDebug() << "IP address or port is not set. Cannot initialize server listener.";
    }
}

void RobotMainMenu::setTotalLineNumber(int totalLineNumber){
    this->totalLine = totalLineNumber;
}

void RobotMainMenu::showLoadingBar(int value) {

    if(value == -1){
        loadingProgressBar->setVisible(false);
        textLabel->setText(QString("Trying to connect the device... IP: %1 Port: %2")
                                  .arg(ipAddress)
                                  .arg(port));

        loadingLabel->setVisible(true); // Hide the loading movie
        textLabel->setVisible(true);
        loadingMovie->start(); // Start the loading circle animation
    }
    else{
        loadingMovie->stop(); // Stop the loading circle animation
        loadingLabel->setVisible(false); // Hide the loading movie
        textLabel->setVisible(false);
        if(this->totalLine > 0){
            loadingProgressBar->setValue((value * 100) / this->totalLine); // Set the loading bar value
        }
        //loadingProgressBar->setValue((value / this->totalLine)); // Set the loading bar value
        //loadingProgressBar->setValue(value);
        loadingProgressBar->setVisible(true); // Show the loading bar
    }
}

void RobotMainMenu::disconnectFromServer(){
    if (serverListenerThread) {
        serverListenerThread->requestInterruption();
        serverListenerThread->quit();
        serverListenerThread->wait();
        loadingProgressBar->setVisible(false);
        textLabel->setText("Device is not connected!");
        loadingLabel->setVisible(false);
        textLabel->setVisible(true);
    }
}

QString RobotMainMenu::getIP(){
    return ipAddress;
}

int RobotMainMenu::getPort(){
    return port;
}

