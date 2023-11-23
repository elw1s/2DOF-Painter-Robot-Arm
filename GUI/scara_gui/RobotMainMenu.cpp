#include "RobotMainMenu.h"

RobotMainMenu::RobotMainMenu(QWidget *parent) : QWidget(parent) {
    //timeData = new QVector();
    //sensorData = new QVector();

    label = new QLabel("Preview of drawing");
    projectionWidget = new RobotProjectionWidget(this);
    bottomButton = new QPushButton("View 3D Virtualization");
    bottomButton->setObjectName("bottomButton");
    bottomButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bottomButton->setFixedWidth(250);
    bottomButton->setFixedHeight(50); // Set the fixed height
    bottomButton->setStyleSheet("QPushButton#bottomButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 75px;"
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
    sensorPlot->xAxis->setLabel("Time");
    sensorPlot->yAxis->setLabel("Sensor Value");
    sensorPlot->xAxis->setRange(0, 100); // Modify the range as needed
    sensorPlot->yAxis->setRange(-100, 100); // Modify the range as needed
    sensorPlot->setBackground(QBrush(QColor("#1C1C1C")));
    sensorPlot->xAxis->setTickLabelColor(Qt::white);
    sensorPlot->yAxis->setTickLabelColor(Qt::white);
    sensorPlot->xAxis->setLabelColor(Qt::white);
    sensorPlot->yAxis->setLabelColor(Qt::white);
    QPen graphPen;
    graphPen.setColor(Qt::red); // Set the line color to red
    sensorPlot->graph(0)->setPen(graphPen);
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("hh:mm:ss"); // Set tick label format
    dateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssReadability);
    sensorPlot->xAxis->setTicker(dateTimeTicker); // Set ticker
    rightTopLayout->addWidget(sensorPlot);

    // Increase width and height of sensorPlot (QCustomPlot)
    sensorPlot->setMinimumSize(150, 130); // Set the minimum size for the QCustomPlot
    sensorPlot->setMaximumSize(300,280);
    // Modify size policies to control how the widgets expand in the layout
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    projectionWidget->setSizePolicy(sizePolicy);
    sensorPlot->setSizePolicy(sizePolicy);


    QVBoxLayout *rightMiddleLayout = new QVBoxLayout;
    // Add 2D plot widgets here

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(rightTopLayout);
    rightLayout->addLayout(rightMiddleLayout);
    rightLayout->addStretch(); // Add a stretch before the bottom button
    rightLayout->addWidget(bottomButton);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    ipAddress = QString();
    port = 0;
    initializeServerListener();
}

void RobotMainMenu::updateSensorGraph(double sensorValue) {
    // Add the new data point to the vectors
    QDateTime currentDateTime = QDateTime::currentDateTime(); // Get current date and time
    timeData.append(currentDateTime.toMSecsSinceEpoch() / 1000.0); // Convert QDateTime to seconds since epoch
    sensorData.append(sensorValue);

    // Update the graph with the new data
    sensorPlot->graph(0)->setData(timeData, sensorData);
    sensorPlot->xAxis->setRange(timeData.first(), timeData.last()); // Set range based on timeData
    sensorPlot->xAxis->setLabel("Time (" + currentDateTime.toString("hh:mm:ss") + ")"); // Set label with current time
    sensorPlot->replot(); // Redraw the plot
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
        connect(serverListenerThread, &ServerListenerThread::totalLineNumber, this, &RobotMainMenu::setTotalLineNumber);
        connect(serverListenerThread, &ServerListenerThread::loadingProgress, this, &RobotMainMenu::showLoadingBar);
        connect(serverListenerThread, &ServerListenerThread::sensorValues,
                this, &RobotMainMenu::updateSensorGraph);
        serverListenerThread->start();
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

