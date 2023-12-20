#include "DrawingApp.h"


DrawingApp::DrawingApp(QWidget *parent) : QWidget(parent) {
    drawingArea = new DrawingArea(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(2); // Set spacing between buttons to 0
    topLayout->setAlignment(Qt::AlignLeft); // Align buttons to the left

    saveButton = new QPushButton("Draw", this);
    saveButton->setObjectName("saveButton"); // Set object name to apply specific styles
    connect(saveButton, &QPushButton::clicked, this, &DrawingApp::saveImage);

    mainLayout->setContentsMargins(25, 75, 75, 20); // Adjust the margins as needed

    QSize iconSize(35, 35);

    QIcon penIcon(QString::fromStdString(PEN_ICON));
    penButton = createStyledButton(penIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(penButton, &QPushButton::clicked, this, &DrawingApp::setEraserFalse);
    topLayout->addWidget(penButton);

    QIcon eraserIcon(QString::fromStdString(ERASER_ICON));
    eraserButton = createStyledButton(eraserIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(eraserButton, &QPushButton::clicked, this, &DrawingApp::setEraserTrue);
    topLayout->addWidget(eraserButton);

    QIcon resetIcon(QString::fromStdString(RESET_ICON));
    resetButton = createStyledButton(resetIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(resetButton, &QPushButton::clicked, this, &DrawingApp::resetDrawing);
    topLayout->addWidget(resetButton);


    penStrokeDropdown = new QComboBox(this);
    penStrokeDropdown->addItem(QIcon(QString::fromStdString(STROKE_ICON)), "", QVariant::fromValue(iconSize));
    penStrokeDropdown->addItem("1");
    penStrokeDropdown->addItem("2");
    penStrokeDropdown->addItem("3");
    penStrokeDropdown->addItem("4");
    penStrokeDropdown->setCurrentIndex(0);

    penStrokeDropdown->setStyleSheet("QComboBox { background-color: #1C1C1C; border: 1px solid #767676; color: #FFFFFF; }");

    connect(penStrokeDropdown, QOverload<int>::of(&QComboBox::activated), this, &DrawingApp::setPenStroke);

    QSize dropdownSize = iconSize; // Assuming you want the dropdown to have the same size as the icons
    penStrokeDropdown->setFixedWidth(dropdownSize.width());
    penStrokeDropdown->setFixedHeight(dropdownSize.height());

    topLayout->addWidget(penStrokeDropdown);

    mainLayout->addLayout(topLayout);

    drawingArea->setStyleSheet("background-color: #D9D9D9; margin: 75px 75px 20px 25px;");

    mainLayout->addWidget(drawingArea);

    // Customize Save Image button
    saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    saveButton->setFixedWidth(200);
    saveButton->setFixedHeight(50); // Set the fixed height
    saveButton->setStyleSheet("QPushButton#saveButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 0px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#saveButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Push the button to the right
    buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout);

    mainLayout->setAlignment(Qt::AlignRight | Qt::AlignTop); // Align the layout to the top-right

    setLayout(mainLayout);
}

void DrawingApp::resetDrawing() {
    drawingArea->clearDrawing(); // Assuming you have a function to clear the drawing in DrawingArea
}

void DrawingApp::saveImage() {
    QDir dir;
    dir = dir.temp();
    QString filePath = dir.path() + "/cse396/image.jpg";
    drawingArea->getPixmap().toImage().save(filePath, "JPG");
    emit drawButtonClicked();

}


void DrawingApp::setPenStroke(int stroke) {
        // Assuming DrawingArea has a function to set the pen stroke
        drawingArea->setPenStroke(stroke + 3); // +1 to align with the dropdown (index 0-3) and stroke values (1-4)
}

void DrawingApp::setEraserTrue(){
    if(drawingArea->isEraserActive()){

        eraserButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
        penButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
        drawingArea->eraserMode(false);
        drawingArea->penMode(false);
    }
    else{
        eraserButton->setStyleSheet("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
        penButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
        drawingArea->eraserMode(true);
        drawingArea->penMode(false);
    }
}

void DrawingApp::setEraserFalse(){
    if(drawingArea->isPenActive()){
            penButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
            eraserButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
            drawingArea->eraserMode(false);
            drawingArea->penMode(false);
    }
    else{
        penButton->setStyleSheet("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
        eraserButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
        drawingArea->eraserMode(false);
        drawingArea->penMode(true);
    }
}

QPushButton* DrawingApp::createStyledButton(const QIcon &icon, const QSize &size, const QString &textColor,
                                            const QString &borderColor, QWidget *parent) {
        QPushButton *button = new QPushButton(parent);
        button->setIcon(icon);
        button->setIconSize(size);
        button->setFixedSize(size);

        // Initial button style
        button->setStyleSheet(QString("border: 1px solid %1; background-color: rgba(28, 28, 28, 0); color: %2;")
                                  .arg(borderColor)
                                  .arg(textColor));

        // Change border color when button is pressed
        button->setProperty("borderColor", borderColor); // Store original border color
        connect(button, &QPushButton::pressed, [=]() {
            button->setStyleSheet(QString("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: %1;")
                                      .arg(textColor));
        });

        // Revert to original border color when button is released
        connect(button, &QPushButton::released, [=]() {
            QString storedBorderColor = button->property("borderColor").toString();
            button->setStyleSheet(QString("border: 1px solid %1; background-color: rgba(28, 28, 28, 0); color: %2;")
                                      .arg(storedBorderColor)
                                      .arg(textColor));
        });

        return button;
}

//Freeze the app
void DrawingApp::robotDrawingSignal(const bool status){
    if(status){
        saveButton->setEnabled(false);
        saveButton->setStyleSheet("QPushButton#saveButton {"
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
        saveButton->setEnabled(true);
        saveButton->setStyleSheet("QPushButton#saveButton {"
                                  "    background-color: #33C2FF;"
                                  "    color: #424242;"
                                  "    font-family: Abel;"
                                  "    font-size: 12px;"
                                  "    border: 1px solid #767676;"
                                  "    margin-right: 0px;"
                                  "    margin-bottom: 5px;"
                                  "}"
                                  "QPushButton#saveButton:hover {"
                                  "    background-color: #57D5FF;" // Change color on hover if desired
                                  "}");
    }
}

void DrawingApp::setColors(const QList<QString>& colorArr) {
    this->colors = colorArr;

    qDebug() << "DRAWING APP Colors:";
    for(int i = 0; i < 4; i++){
        qDebug() << this->colors[i];
    }
}

QList<QString> DrawingApp::getColors(){
    return colors;
}

