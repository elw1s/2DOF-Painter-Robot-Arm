#include "RobotMainMenu.h"

RobotMainMenu::RobotMainMenu(QWidget *parent) : QWidget(parent) {
    //timeData = new QVector();
    //sensorData = new QVector();

    //label = new QLabel("Preview of drawing");
    //label->setStyleSheet("color: white;");
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
    textLabel->setStyleSheet("color: white;");
    loadingLabel = new QLabel(this);
    loadingLabel->setStyleSheet("color: white;");
    loadingMovie = new QMovie(QString::fromStdString(LOADING_GIF)); // Provide the path to your loading GIF
    loadingMovie->setScaledSize(QSize(20, 20)); // Set the size of the movie (GIF)
    loadingLabel->setMovie(loadingMovie);
    loadingLabel->setVisible(false);

    loadingProgressBar = new QProgressBar(this);
    loadingProgressBar->setTextVisible(true);
    loadingProgressBar->setMinimum(0);
    loadingProgressBar->setMaximum(100);
    loadingProgressBar->setVisible(false); // Initially hidden
    loadingProgressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid #19749B;"
        "    border-radius: 5px;"
        "    background-color: #1C1C1C;"
        "    text-align: center;" // Center align the text in the progress bar
        "}"
        "QProgressBar::chunk {"
        "    background-color: #33C2FF;"
        "    width: 10px;"
        "}"
        );

    loadingLabelLayout->addWidget(loadingLabel);
    loadingLabelLayout->addWidget(textLabel);
    loadingLabelLayout->setAlignment(Qt::AlignLeft);

    // Add loading elements below the projection widget
    QVBoxLayout *leftLayout = new QVBoxLayout;
    //leftLayout->addWidget(label,10,Qt::AlignCenter);
    leftLayout->addLayout(loadingLabelLayout);
    leftLayout->addWidget(scrollArea);
    //leftLayout->addWidget(loadingLabel); // Add the loading circle widget
    //leftLayout->addLayout(loadingLabelLayout);
    leftLayout->addWidget(loadingProgressBar); // Add the progress bar widget

    QVBoxLayout *rightTopLayout = new QVBoxLayout;
    sensorPlot  = new QCustomPlot();
    sensorPlot->addGraph();
    sensorPlot->yAxis->setLabel("Sensor Value (Acceleration X axis)");
    sensorPlot->yAxis->setRange(-1, 1); // Modify the range as needed
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
    sensorPlot->setMaximumSize(450,400);
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
    waveformPlot->setMaximumSize(450,400);
    waveformPlot->setSizePolicy(sizePolicy);
    waveformPlot->xAxis->setTicker(dateTimeTicker); // Set ticker


    waveformPlot->xAxis->setTickLabels(false);

    // Add graphs for each line
    waveformPlot->addGraph();
    waveformPlot->graph(0)->setPen(QPen(QColor("#ed4046"), 2)); // Red line

    waveformPlot->addGraph();
    waveformPlot->graph(1)->setPen(QPen(QColor("#327ba8"), 2)); // Blue line with color #327ba8

    //waveformPlot->addGraph();
    //waveformPlot->graph(2)->setPen(QPen(QColor("#1bb310"), 2)); // Green line

    waveformPlot->xAxis->setTickPen(QPen(QColor("#33C2FF"))); // Major ticks on x-axis
    waveformPlot->xAxis->setSubTickPen(QPen(QColor("#19749B"))); // Minor ticks on x-axis
    waveformPlot->yAxis->setTickPen(QPen(QColor("#33C2FF"))); // Major ticks on y-axis
    waveformPlot->yAxis->setSubTickPen(QPen(QColor("#19749B"))); // Minor ticks on y-axis

    // Set up the layout for the waveform plot and its label
    QVBoxLayout *waveformLayout = new QVBoxLayout;
    waveformLayout->addWidget(waveformPlot); // Add the waveform plot to the layout

    // Add label for color and value key
    QHBoxLayout *legendLabelLayout = new QHBoxLayout;
    legendLabelLayout->setSpacing(20); // Adjust the spacing between items
    redLabel = new QLabel();
    redLabel->setText("Shoulder Motor: 0");
    blueLabel = new QLabel();
    blueLabel->setText("Elbow Motor: 0");
    greenLabel = new QLabel();
    greenLabel->setText("Pen: UP");

    redLabel->setStyleSheet("color: #ed4046;"); // Style the label as needed
    blueLabel->setStyleSheet("color: #327ba8;"); // Style the label as needed
    greenLabel->setStyleSheet("color: #1bb310;"); // Style the label as needed
    legendLabelLayout->addWidget(redLabel);
    legendLabelLayout->addWidget(blueLabel);
    legendLabelLayout->addWidget(greenLabel);
    legendLabelLayout->setAlignment(Qt::AlignCenter); // Center align the labels within the layout
    waveformLayout->addLayout(legendLabelLayout);
    rightMiddleLayout->addLayout(waveformLayout);

    //rightMiddleLayout->setContentsMargins(0, 0, 0, 0); // Adjust margins if needed

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(rightTopLayout);
    rightLayout->addLayout(rightMiddleLayout);

    QVBoxLayout *sliderLayout = new QVBoxLayout;

    // Slider 1 (Red)
    sliderRed = new QSlider(Qt::Horizontal);
    sliderRed->setStyleSheet(
        "QSlider::groove:horizontal { border: 1px solid #ed4046; background: #ed4046; height: 4px; border-radius: 2px; }"
        "QSlider::handle:horizontal { background-color: #ed4046; border: 1px solid #ffffff; width: 10px; margin: -5px; border-radius: 5px; }"
        "QSlider::handle:horizontal:hover { background-color: #ff0000; border: 1px solid #ffffff; }"
        );
    sliderRed->setRange(-90, 90); // Modify the range as needed
    sliderRed->setSingleStep(1); // Set the step size for sliderRed
    sliderRed->setPageStep(10); // Set the page step size for sliderRed
    sliderRed->setSliderPosition(-90); // Set initial slider position
    sliderRed->setMinimumSize(150, 60);  // Set minimum size for sliderRed
    sliderRed->setMaximumSize(400, 150);  // Set maximum size for sliderRed
    sliderRed->setTickInterval(10);
    sliderRed->setTickPosition(QSlider::TicksBelow); // Set the tick position to display ticks below the slider
    QLabel *labelRed = new QLabel("-90");
    labelRed->setStyleSheet("color: #ed4046;");

    sliderLayout->addWidget(sliderRed);
    sliderLayout->addWidget(labelRed, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Slider 2 (Blue)
    sliderBlue = new QSlider(Qt::Horizontal);
    sliderBlue->setStyleSheet(
        "QSlider::groove:horizontal { border: 1px solid #327ba8; background: #327ba8; height: 4px; border-radius: 2px; }"
        "QSlider::handle:horizontal { background-color: #327ba8; border: 1px solid #ffffff; width: 10px; margin: -5px; border-radius: 5px; }"
        "QSlider::handle:horizontal:hover { background-color: #ff0000; border: 1px solid #ffffff; }"
        );
    sliderBlue->setRange(0, 120); // Modify the range as needed
    sliderBlue->setSingleStep(1); // Set the step size for sliderRed
    sliderBlue->setPageStep(10); // Set the page step size for sliderRed
    sliderBlue->setSliderPosition(90); // Set initial slider position
    sliderBlue->setMinimumSize(150, 60);  // Set minimum size for sliderRed
    sliderBlue->setMaximumSize(400, 150);  // Set maximum size for sliderRed
    sliderBlue->setTickInterval(10);
    sliderBlue->setTickPosition(QSlider::TicksBelow); // Set the tick position to display ticks below the slider
    QLabel *labelBlue = new QLabel("90");
    labelBlue->setStyleSheet("color: #327ba8;");

    sliderLayout->addWidget(sliderBlue);
    sliderLayout->addWidget(labelBlue, 0, Qt::AlignTop | Qt::AlignHCenter);


    // Slider 3 (Green)
//    QSlider *sliderGreen = new QSlider(Qt::Horizontal);
//    sliderGreen->setStyleSheet(
//        "QSlider::groove:horizontal { border: 1px solid #1bb310; background: #1bb310; height: 4px; border-radius: 2px; }"
//        "QSlider::handle:horizontal { background-color: #1bb310; border: 1px solid #ffffff; width: 10px; margin: -5px; border-radius: 5px; }"
//        "QSlider::handle:horizontal:hover { background-color: #ff0000; border: 1px solid #ffffff; }"
//        );
//    sliderGreen->setRange(0, 100); // Modify the range as needed
//    sliderGreen->setSingleStep(1); // Set the step size for sliderRed
//    sliderGreen->setPageStep(10); // Set the page step size for sliderRed
//    sliderGreen->setSliderPosition(70); // Set initial slider position
//    sliderGreen->setMinimumSize(150, 60);  // Set minimum size for sliderRed
//    sliderGreen->setMaximumSize(400, 150);  // Set maximum size for sliderRed
//    sliderGreen->setTickInterval(10);
//    sliderGreen->setTickPosition(QSlider::TicksBelow); // Set the tick position to display ticks below the slider
//    QLabel *labelGreen = new QLabel("70");
//    labelGreen->setStyleSheet("color: #1bb310;");

//    sliderLayout->addWidget(sliderGreen);
//    sliderLayout->addWidget(labelGreen, 0, Qt::AlignTop | Qt::AlignHCenter);


    connect(sliderRed, &QSlider::valueChanged, this, [=](int value) {
        labelRed->setText(QString("%1").arg(value));
        // Do something with the red slider value (value)
    });

    connect(sliderBlue, &QSlider::valueChanged, this, [=](int value) {
        labelBlue->setText(QString("%1").arg(value));
        // Do something with the blue slider value (value)
    });

//    connect(sliderGreen, &QSlider::valueChanged, this, [=](int value) {
//        labelGreen->setText(QString("%1").arg(value));
//        // Do something with the green slider value (value)
//    });
    sliderLayout->setSpacing(0); // Adjust the spacing between widgets in the layout
    sliderLayout->setContentsMargins(0, 0, 0, 20); // Adjust margins if needed
    rightLayout->addLayout(sliderLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    setButton = new QPushButton("Set Angles", this);
    setButton->setObjectName("setButton"); // Set object name to apply specific styles
    connect(setButton, &QPushButton::clicked, this, &RobotMainMenu::setButtonClicked);
    setButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setButton->setFixedWidth(100);
    setButton->setFixedHeight(50); // Set the fixed height
    setButton->setStyleSheet("QPushButton#setButton {"
                              "    background-color: #33C2FF;"
                              "    color: white;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 0px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#setButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");

    defaultButton = new QPushButton("Calibrate", this);
    defaultButton->setObjectName("defaultButton"); // Set object name to apply specific styles
    connect(defaultButton, &QPushButton::clicked, this, &RobotMainMenu::defaultButtonClicked);
    defaultButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    defaultButton->setFixedWidth(150);
    defaultButton->setFixedHeight(50); // Set the fixed height
    defaultButton->setStyleSheet("QPushButton#defaultButton {"
                             "    background-color: #33C2FF;"
                             "    color: white;"
                             "    font-family: Abel;"
                             "    font-size: 12px;"
                             "    border: 1px solid #767676;"
                             "    margin-right: 0px;"
                             "    margin-bottom: 5px;"
                             "}"
                             "QPushButton#defaultButton:hover {"
                             "    background-color: #57D5FF;" // Change color on hover if desired
                             "}");

    buttonLayout->addWidget(defaultButton);
    buttonLayout->addWidget(setButton);


    rightLayout->setSpacing(0);
    rightLayout->setContentsMargins(0, 0, 0, 0); // Adjust margins if needed
    rightLayout->addLayout(buttonLayout);


    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    updateTimerServo = new QTimer(this);
    connect(updateTimerServo, &QTimer::timeout, this, &RobotMainMenu::onUpdateTimerServo);
    updateTimerSensor = new QTimer(this);
    connect(updateTimerSensor, &QTimer::timeout, this, &RobotMainMenu::onUpdateTimerSensor);

    ipAddress = QString();
    port = 0;
    colors << QString("+") << QString("+") << QString("+") << QString("+");
    serverListenerThread = nullptr;
    initializeServerListener();
    }


void RobotMainMenu::setButtonClicked(){
    if(serverListenerThread && serverListenerThread->isConnected()){
        serverListenerThread->move(true,sliderRed->value(), sliderBlue->value());
    }
}

void RobotMainMenu::defaultButtonClicked(){
    if(serverListenerThread && serverListenerThread->isConnected()){
        serverListenerThread->move(true,-90, 90);
        sliderRed->setValue(-90);
        sliderBlue->setValue(90);
    }
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
    QString greenLabelText;
    if(thirdAngle == 1700){
        greenLabelText = QString("PEN: DOWN");
    }
    else if(thirdAngle == 1200){
        greenLabelText = QString("PEN: UP");
    }


    redLabel->setText(redLabelText);
    blueLabel->setText(blueLabelText);
    greenLabel->setText(greenLabelText);


    // Update each graph with its respective data
    //if(updateTimer->remainingTime() <= 0){
        waveformPlot->graph(0)->setData(timeDataServo, firstAngleData); // Green line
        waveformPlot->graph(1)->setData(timeDataServo, secondAngleData); // Red line
        //waveformPlot->graph(2)->setData(timeDataServo, thirdAngleData); // Blue line

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

    emit sendServoAngles(firstAngle, secondAngle, thirdAngle);
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
        connect(serverListenerThread, &ServerListenerThread::drawingStatus, this, &RobotMainMenu::robotDrawingSignal);

        connect(serverListenerThread, &ServerListenerThread::stats_last_packet_size, this, &RobotMainMenu::stats_last_packet_size_slot);
        connect(serverListenerThread, &ServerListenerThread::stats_number_of_drawn_line, this, &RobotMainMenu::stats_number_of_drawn_line_slot);
        connect(serverListenerThread, &ServerListenerThread::stats_number_of_lines_to_draw, this, &RobotMainMenu::stats_number_of_lines_to_draw_slot);
        connect(serverListenerThread, &ServerListenerThread::stats_received_packet_num, this, &RobotMainMenu::stats_received_packet_num_slot);
        connect(serverListenerThread, &ServerListenerThread::stages, this, &RobotMainMenu::stagesTextLabel);
        serverListenerThread->start();
        updateTimerSensor->start(1000);
        updateTimerServo->start(1000);

    } else {
        qDebug() << "IP address or port is not set. Cannot initialize server listener.";
    }
}

void RobotMainMenu::stats_received_packet_num_slot(const int received_packet_num){
    emit stats_received_packet_num(received_packet_num);
}

void RobotMainMenu::stats_last_packet_size_slot(const double last_packet_size){
    emit stats_last_packet_size(last_packet_size);
}

void RobotMainMenu::stats_number_of_drawn_line_slot(const int number_of_drawn_line){
    emit stats_number_of_drawn_line(number_of_drawn_line);
}

void RobotMainMenu::stats_number_of_lines_to_draw_slot(const int number_of_lines_to_draw){
    emit stats_number_of_lines_to_draw(number_of_lines_to_draw);
}

void RobotMainMenu::robotDrawingSignal(const bool status){
    emit drawingStatus(status);
    if(status){
        projectionWidget->clear();
        defaultButton->setEnabled(false);
        defaultButton->setStyleSheet("QPushButton#defaultButton {"
                                  "    background-color: #4F4F4F;"
                                  "    color: #DEDEDE;"
                                  "    font-family: Abel;"
                                  "    font-size: 12px;"
                                  "    border: 1px solid #767676;"
                                  "    margin-right: 0px;"
                                  "    margin-bottom: 5px;"
                                  "}"
                                  );
        setButton->setEnabled(false);
        setButton->setStyleSheet("QPushButton#setButton {"
                                  "    background-color: #4F4F4F;"
                                  "    color: #DEDEDE;"
                                  "    font-family: Abel;"
                                  "    font-size: 12px;"
                                  "    border: 1px solid #767676;"
                                  "    margin-right: 0px;"
                                  "    margin-bottom: 5px;"
                                  "}"
                                  );
    }
    else{
        setButton->setEnabled(true);
        defaultButton->setEnabled(true);
        setButton->setStyleSheet("QPushButton#setButton {"
                                 "    background-color: #33C2FF;"
                                 "    color: white;"
                                 "    font-family: Abel;"
                                 "    font-size: 12px;"
                                 "    border: 1px solid #767676;"
                                 "    margin-right: 0px;"
                                 "    margin-bottom: 5px;"
                                 "}"
                                 "QPushButton#setButton:hover {"
                                 "    background-color: #57D5FF;" // Change color on hover if desired
                                 "}");
        defaultButton->setStyleSheet("QPushButton#defaultButton {"
                                     "    background-color: #33C2FF;"
                                     "    color: white;"
                                     "    font-family: Abel;"
                                     "    font-size: 12px;"
                                     "    border: 1px solid #767676;"
                                     "    margin-right: 0px;"
                                     "    margin-bottom: 5px;"
                                     "}"
                                     "QPushButton#defaultButton:hover {"
                                     "    background-color: #57D5FF;" // Change color on hover if desired
                                     "}");
    }

}

//Eğer server bağlı değilse draw butonuna tıklanılmasın
void RobotMainMenu::drawButtonClicked(){
    if(serverListenerThread && serverListenerThread->isConnected()){
        serverListenerThread->draw(true);
    }
}

void RobotMainMenu::moveButtonClicked(){
    if(serverListenerThread && serverListenerThread->isConnected()){
        //serverListenerThread->move(true,0,0,0); //Burada açılar değiştirilecek.
    }
}

void RobotMainMenu::setTotalLineNumber(int totalLineNumber){
    this->totalLine = totalLineNumber;
}

void RobotMainMenu::stagesTextLabel(int value){
    if(value == 1){
        textLabel->setText(QString("Sending image to the robot..."));
        loadingMovie = new QMovie(QString::fromStdString(UPLOAD_GIF)); // Provide the path to your loading GIF
        loadingMovie->setScaledSize(QSize(20, 20)); // Set the size of the movie (GIF)
        loadingLabel->setMovie(loadingMovie);
        loadingLabel->setVisible(true);
        loadingMovie->start();
    }
    else if(value == 2){
        textLabel->setText(QString("Vectorizing image..."));
        loadingMovie = new QMovie(QString::fromStdString(IMAGE_PROCESS_GIF)); // Provide the path to your loading GIF
        loadingMovie->setScaledSize(QSize(20, 20)); // Set the size of the movie (GIF)
        loadingLabel->setMovie(loadingMovie);
        loadingLabel->setVisible(true);
        loadingMovie->start();
    }
    else if(value == 3){
        textLabel->setText(QString("Image process finished..."));
        loadingMovie = new QMovie(QString::fromStdString(DONE_GIF)); // Provide the path to your loading GIF
        loadingMovie->setScaledSize(QSize(20, 20)); // Set the size of the movie (GIF)
        loadingLabel->setMovie(loadingMovie);
        loadingLabel->setVisible(true);
        loadingMovie->start();
    }
    else{
        textLabel->setText(QString("Device is connected... IP: %1 Port: %2")
                               .arg(ipAddress)
                               .arg(port));
        loadingLabel->setVisible(false); // Hide the loading movie
        loadingMovie->stop();
    }
}

void RobotMainMenu::showLoadingBar(int value) {

    if(value == -1){
        //loadingProgressBar->setVisible(false);
        textLabel->setText(QString("Trying to connect the device... IP: %1 Port: %2")
                                  .arg(ipAddress)
                                  .arg(port));

        loadingMovie = new QMovie(QString::fromStdString(LOADING_GIF)); // Provide the path to your loading GIF
        loadingMovie->setScaledSize(QSize(20, 20)); // Set the size of the movie (GIF)
        loadingLabel->setMovie(loadingMovie);
        loadingLabel->setVisible(true); // Hide the loading movie
        textLabel->setVisible(true);
        loadingMovie->start(); // Start the loading circle animation
    }
    else if(value == 0){
        textLabel->setText(QString("Device is connected... IP: %1 Port: %2")
                               .arg(ipAddress)
                               .arg(port));
        textLabel->setVisible(true);
        loadingLabel->setVisible(false); // Hide the loading movie
        loadingMovie->stop();
    }
    else{
        //loadingMovie->stop(); // Stop the loading circle animation
        //loadingLabel->setVisible(false); // Hide the loading movie
        //textLabel->setVisible(false);
        qDebug() << "value=" << value << " **this->totalLine="<< this->totalLine;
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
        projectionWidget->clear();
        serverListenerThread->requestInterruption();
        serverListenerThread->quit();
        serverListenerThread->wait();
        serverListenerThread->socketDisconnected();
        textLabel->setText("Device is not connected!");
        loadingLabel->setVisible(false); // Hide the loading movie
        loadingMovie->stop();
        serverListenerThread = nullptr;
        this->ipAddress = "";
        this->port = 0;
    }
}

QString RobotMainMenu::getIP(){
    return ipAddress;
}

int RobotMainMenu::getPort(){
    return port;
}

void RobotMainMenu::setColors(const QList<QString>& colorArr) {
    this->colors = colorArr;
}

QList<QString> RobotMainMenu::getColors(){
    return colors;
}

