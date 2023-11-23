#include "Settings.h"

Settings::Settings(const QString& ipAddress, int port, QWidget *parent)
    : QDialog(parent), mIpAddress(ipAddress), mPort(port) {
    setWindowTitle("Settings");
    setFixedSize(800, 600); // Set your desired size

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* leftLayout = new QVBoxLayout;
    QVBoxLayout* rightLayout = new QVBoxLayout;
    QVBoxLayout* buttonLayout = new QVBoxLayout(this);

    mainLayout->setAlignment(Qt::AlignCenter);

    labelIP = new QLabel("Enter IP Address", this);
    labelPort = new QLabel("Enter Port", this);
    lineEditIP = new QLineEdit(this);
    lineEditPort = new QLineEdit(this);
    connectButton = new QPushButton("Connect", this);
    disconnectButton = new QPushButton("Disconnect", this);
    connectButton->setObjectName("connectButton"); // Set object name to apply specific styles

    connectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connectButton->setFixedWidth(250);
    connectButton->setFixedHeight(50); // Set the fixed height
    connectButton->setStyleSheet("QPushButton#connectButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 75px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#connectButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");

    disconnectButton->setObjectName("disconnectButton"); // Set object name to apply specific styles

    disconnectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    disconnectButton->setFixedWidth(250);
    disconnectButton->setFixedHeight(50); // Set the fixed height
    disconnectButton->setStyleSheet("QPushButton#disconnectButton {"
                                 "    background-color: #33C2FF;"
                                 "    color: #424242;"
                                 "    font-family: Abel;"
                                 "    font-size: 12px;"
                                 "    border: 1px solid #767676;"
                                 "    margin-right: 75px;"
                                 "    margin-bottom: 5px;"
                                 "}"
                                 "QPushButton#disconnectButton:hover {"
                                 "    background-color: #bf12064A;" // Change color on hover if desired
                                 "}");


    labelIP->setStyleSheet("color: #33C2FF;");
    labelPort->setStyleSheet("color: #33C2FF;");
    lineEditIP->setStyleSheet("border: 1px solid #33C2FF;");
    lineEditPort->setStyleSheet("border: 1px solid #33C2FF;");

    // Prevent expansion
    connectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    disconnectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lineEditIP->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lineEditPort->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(connectButton, &QPushButton::clicked, this, &Settings::onOKButtonClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &Settings::onDisconnectButtonClicked);

    if(mPort == 0){
        disconnectButton->setEnabled(false);
        disconnectButton->setStyleSheet("QPushButton#disconnectButton {"
                                        "    background-color: #4F4F4F;"
                                        "    color: #424242;"
                                        "    font-family: Abel;"
                                        "    font-size: 12px;"
                                        "    border: 1px solid #767676;"
                                        "    margin-right: 75px;"
                                        "    margin-bottom: 5px;"
                                        "}"
                                        );
    }
    else{
        connectButton->setStyleSheet("QPushButton#connectButton {"
                                     "    background-color: #4F4F4F;"
                                     "    color: #424242;"
                                     "    font-family: Abel;"
                                     "    font-size: 12px;"
                                     "    border: 1px solid #767676;"
                                     "    margin-right: 75px;"
                                     "    margin-bottom: 5px;"
                                     "}");
        connectButton->setEnabled(false);
    }

    QString contributorsText = "Contributors\n\nKahraman Arda KILIÇ\nErsel Celal Eren\nHalil İbrahim İlhan\nÖzlem Malkoç\nTaylan Yerlikaya\nMustafa Mercan\nHüseyin Emre Sekanlı";
    QLabel* contributorsLabel = new QLabel(contributorsText, this);
    contributorsLabel->setStyleSheet("color: #DEDEDE;"); // Set text color
    contributorsLabel->setWordWrap(true); // Allow word wrap
    contributorsLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter); // Set alignment

    // Set background color
    this->setStyleSheet("background-color: #1C1C1C;");

    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(disconnectButton);
    buttonLayout->setContentsMargins(0,20,0,0);

    // Add widgets to layout
    leftLayout->addWidget(labelIP);
    leftLayout->addWidget(lineEditIP);
    leftLayout->addWidget(labelPort);
    leftLayout->addWidget(lineEditPort);
    leftLayout->addLayout(buttonLayout);
    leftLayout->setAlignment(Qt::AlignHCenter | Qt::AlignRight);
    rightLayout->addWidget(contributorsLabel);

    mainLayout->addLayout(leftLayout, 65); // 65% width for leftLayout
    mainLayout->addLayout(rightLayout, 35); // 35% width for rightLayout
    setLayout(mainLayout);
}

void Settings::onOKButtonClicked() {
    QString ipAddress = lineEditIP->text().trimmed(); // Get the entered IP address
    QString portString = lineEditPort->text().trimmed(); // Get the entered port number as a string

    int port = portString.toInt();

    connectButton->setEnabled(false);
    disconnectButton->setEnabled(true);
    connectButton->setStyleSheet("QPushButton#connectButton {"
                                 "    background-color: #4F4F4F;"
                                 "    color: #424242;"
                                 "    font-family: Abel;"
                                 "    font-size: 12px;"
                                 "    border: 1px solid #767676;"
                                 "    margin-right: 75px;"
                                 "    margin-bottom: 5px;"
                                 "}"
                                 );
    disconnectButton->setStyleSheet("QPushButton#disconnectButton {"
                                    "    background-color: #33C2FF;"
                                    "    color: #424242;"
                                    "    font-family: Abel;"
                                    "    font-size: 12px;"
                                    "    border: 1px solid #767676;"
                                    "    margin-right: 75px;"
                                    "    margin-bottom: 5px;"
                                    "}"
                                    "QPushButton#disconnectButton:hover {"
                                    "    background-color: #57D5FF;" // Change color on hover if desired
                                    "}");


    emit settingsUpdated(ipAddress, port);
    accept(); // Close the dialog
}

void Settings::onDisconnectButtonClicked(){
    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
    disconnectButton->setStyleSheet("QPushButton#disconnectButton {"
                                    "    background-color: #4F4F4F;"
                                    "    color: #424242;"
                                    "    font-family: Abel;"
                                    "    font-size: 12px;"
                                    "    border: 1px solid #767676;"
                                    "    margin-right: 75px;"
                                    "    margin-bottom: 5px;"
                                    "}"
                                    );
    connectButton->setStyleSheet("QPushButton#connectButton {"
                                 "    background-color: #33C2FF;"
                                 "    color: #424242;"
                                 "    font-family: Abel;"
                                 "    font-size: 12px;"
                                 "    border: 1px solid #767676;"
                                 "    margin-right: 75px;"
                                 "    margin-bottom: 5px;"
                                 "}"
                                 "QPushButton#connectButton:hover {"
                                 "    background-color: #57D5FF;" // Change color on hover if desired
                                 "}");

    emit disconnectSignal();
}
