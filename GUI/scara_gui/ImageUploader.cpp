#include "ImageUploader.h"

ImageUploader::ImageUploader(QWidget* parent)
{
    colorsIndex = 0;
    numberOfNotDrawnColors = 0;
    drawClicked = false;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(2); // Set spacing between buttons to 0
    topLayout->setAlignment(Qt::AlignLeft); // Align buttons to the left

    mainLayout->setContentsMargins(25, 75, 75, 20); // Adjust the margins as needed

    QSize iconSize(35, 35);

    QIcon addFileIcon(QString::fromStdString(SELECT_FILE_ICON)); // Replace ":/icons/white_pen_icon.png" with your actual icon path
    addFileButton = createStyledButton(addFileIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(addFileButton, &QPushButton::clicked, this, &ImageUploader::onAddFile);
    topLayout->addWidget(addFileButton);

    QIcon cropIcon(QString::fromStdString(CROP_ICON)); // Replace ":/icons/white_pen_icon.png" with your actual icon path
    cropButton = createStyledButton(cropIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(cropButton, &QPushButton::clicked, this, &ImageUploader::onCropButtonClicked);
    topLayout->addWidget(cropButton);

    QIcon rotateIcon(QString::fromStdString(ROTATE_ICON)); // Replace ":/icons/white_pen_icon.png" with your actual icon path
    rotateButton = createStyledButton(rotateIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(rotateButton, &QPushButton::clicked, this, &ImageUploader::onRotateButtonClicked);
    topLayout->addWidget(rotateButton);

    m_graphicsView = new QGraphicsView(this);
    m_clippedLabel = new QLabel(this);

    mainLayout->addLayout(topLayout);
    //mainLayout->addWidget(m_graphicsView);
    //mainLayout->addWidget(m_clippedLabel);

    saveButton = new QPushButton("Draw", this);
    saveButton->setObjectName("saveButton"); // Set object name to apply specific styles
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

    connect(saveButton, &QPushButton::clicked, this, &ImageUploader::saveImage);

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(m_graphicsView);
    bottomLayout->addWidget(m_clippedLabel);
    bottomLayout->addWidget(saveButton);
    bottomLayout->setAlignment(saveButton, Qt::AlignRight); // Align the saveButton to the right within bottomLayout
    mainLayout->addLayout(bottomLayout);
    //mainLayout->addWidget(saveButton);
    mainLayout->setAlignment(Qt::AlignRight | Qt::AlignTop); // Align the layout to the top-right
    m_graphicsView->setBackgroundBrush(QColor("#D9D9D9"));  // Set the background color
    //mainLayout->setContentsMargins(25, 75, 75, 20); // Set the margins
    m_clipScene = new CropFeature(this);
    m_dragdropScene = new DragDropScene(this);
    m_graphicsView->setScene(m_dragdropScene);

    connect(m_clipScene, &CropFeature::clippedImage, this, &ImageUploader::onClippedImage);
    connect(m_clipScene, &CropFeature::rotatedImage, this, &ImageUploader::onRotatedImage);
    connect(m_dragdropScene, &DragDropScene::imageDropped, this, &ImageUploader::onImageDropped);
}

void ImageUploader::onAddFile()
{
    addFileButton->setStyleSheet("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    cropButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    rotateButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        m_clipScene->setImage(pixmap);
        QGraphicsScene *currentScene = m_graphicsView->scene();
        m_graphicsView->setScene(m_clipScene);
        if(currentScene != nullptr){
            //delete currentScene;
        }
    }
    addFileButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    cropButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    rotateButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
}

void ImageUploader::onClippedImage(const QPixmap& pixmap)
{
    m_clipScene->setImage(pixmap);
}

void ImageUploader::onRotatedImage(const QPixmap& pixmap, qreal rotation, QPointF transformOrigin)
{
    m_clipScene->setImage(pixmap, rotation, transformOrigin);
}

void ImageUploader::onCropButtonClicked()
{
    if(m_clipScene->isCropEnabled()){
        cropButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    }
    else{
        cropButton->setStyleSheet("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    }
    addFileButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    rotateButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    m_clipScene->setCropEnabled(!m_clipScene->isCropEnabled());
    m_clipScene->setRotateEnabled(false);
}

void ImageUploader::onRotateButtonClicked()
{
    if(m_clipScene->isRotateEnabled()){
        rotateButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    }
    else{
        rotateButton->setStyleSheet("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    }
    addFileButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    cropButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    m_clipScene->setRotateEnabled(!m_clipScene->isRotateEnabled());
    m_clipScene->setCropEnabled(false);
}

void ImageUploader::onImageDropped() {
    QGraphicsScene *currentScene = m_graphicsView->scene(); // Assuming m_graphicsView points to m_dragdropScene
    QPixmap droppedPixmap = m_dragdropScene->getDroppedPixmap();
    qDebug() << "onImageDropped!";
    // Use the obtained pixmap as needed in m_clipScene (CropFeature)
    if (!droppedPixmap.isNull()) {
        qDebug() << "Dropped image is not null";
        m_clipScene->setImage(droppedPixmap); // Set the dropped pixmap in m_clipScene
        qDebug() << "Image is set in m_clipScene";
        m_graphicsView->setScene(m_clipScene); // Set m_graphicsView to display m_clipScene
        qDebug() << "scene is changed!";
        if(currentScene != nullptr){
            //delete currentScene; // Delete the old scene
        }
        qDebug() << "the other scene is removed!";
    }
}

QPushButton* ImageUploader::createStyledButton(const QIcon &icon, const QSize &size, const QString &textColor,
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

bool ImageUploader::isSimilarColor(const QColor &color1, const QColor &color2, int threshold) {
    // Calculate the differences between color components
    int deltaRed = std::abs(color1.red() - color2.red());
    int deltaGreen = std::abs(color1.green() - color2.green());
    int deltaBlue = std::abs(color1.blue() - color2.blue());

    // Check if each color component difference is within the threshold
    return (deltaRed <= threshold && deltaGreen <= threshold && deltaBlue <= threshold);
}

int ImageUploader::getNumberOfColors(){
    int numberOfColors = 0;
    for(int i = 0; i < this->colors.size(); i++){
        if(this->colors[i]!= "+")
            numberOfColors++;
    }
    return numberOfColors;
}

QString ImageUploader::getNextColor(int start){
    for(int i = start; i < this->colors.size(); i++){
        if(this->colors[i]!= "+"){
            return this->colors[i];
        }
    }
    return QString("#000000");
}


void ImageUploader::applyBorder(const QString filePath) {
    QImage image(filePath); // Load the image
    if (image.isNull()) {
        qDebug() << "Failed to load the image.";
        return; // Exit or handle the failure
    }

    // Define the size of the border (in pixels)
    int borderWidth = 10;

    // Create a painter to draw on the image
    QPainter painter(&image);
    painter.setPen(QPen(Qt::black, 4)); // Set the pen color to black and width to 4 pixels
    painter.setRenderHint(QPainter::Antialiasing); // Optional: Enable anti-aliasing for smoother lines

    // Draw a rectangle border around the image
    painter.drawRect(borderWidth, borderWidth, image.width() - 2 * borderWidth - 4, image.height() - 2 * borderWidth - 4);

    if (!image.save(filePath)) {
        qDebug() << "Failed to save the framed image.";
        return; // Exit or handle the failure
    }
}

void ImageUploader::saveImage() {

    QPixmap croppedPixmap = m_clipScene->getCroppedImage();
    if (!croppedPixmap.isNull()) {
        QDir dir;
        dir = dir.temp();
        QString filePath = dir.path() + "/cse396/image.jpg";
        //bool saved = croppedPixmap.toImage().save(filePath, "JPG");
        //qDebug() << "The image is saved: " << saved;
        //emit drawButtonClicked();

        image = croppedPixmap.toImage();

        numberOfNotDrawnColors = getNumberOfColors();

        if(numberOfNotDrawnColors > 0){
            QColor targetColor(getNextColor(0));
            QImage currImage = image;

            // Loop through each pixel of the image
            for (int y = 0; y < currImage.height(); ++y) {
                for (int x = 0; x < currImage.width(); ++x) {
                    QColor pixelColor(currImage.pixelColor(x, y));

                    // Check for similarity using the defined function
                    if (!pixelColor.operator==(targetColor) && !isSimilarColor(pixelColor, targetColor, 50)) {
                        currImage.setPixelColor(x, y, QColor(Qt::white));
                    }
                    else{
                        image.setPixelColor(x, y, QColor(Qt::white));
                    }
                }
            }
            numberOfNotDrawnColors--;
            colorsIndex++;
            currImage.save(filePath,"JPG");
            applyBorder(filePath);
            drawClicked = true;
            ColorDialog *dialog = new ColorDialog(targetColor, "Please attach the pen in the color below");
            dialog->exec();
        }
        else{
            image.save(filePath, "JPG");
            drawClicked = false;
            numberOfNotDrawnColors = 0;
            colorsIndex = 0;
        }
        emit drawButtonClicked();
    }
}

//Freeze the app
void ImageUploader::robotDrawingSignal(const bool status){
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
        QDir dir;
        dir = dir.temp();
        QString filePath = dir.path() + "/cse396/image.jpg";

        if(numberOfNotDrawnColors > 0){
            //Buradaga image'in üstünde degisiklik yap ve currImage degerini kaydet.
            QColor targetColor(getNextColor(colorsIndex));
            QImage currImage = image;

            // Loop through each pixel of the image
            for (int y = 0; y < currImage.height(); ++y) {
                for (int x = 0; x < currImage.width(); ++x) {
                    QColor pixelColor(currImage.pixelColor(x, y));

                    // Check for similarity using the defined function
                    if (!pixelColor.operator==(targetColor) && !isSimilarColor(pixelColor, targetColor, 50)) {
                        currImage.setPixelColor(x, y, QColor(Qt::white));
                    }
                    else{
                        image.setPixelColor(x, y, QColor(Qt::white));
                    }
                }
            }
            numberOfNotDrawnColors--;
            colorsIndex++;
            currImage.save(filePath,"JPG");
            applyBorder(filePath);
            ColorDialog *dialog = new ColorDialog(targetColor, "Please attach the pen in the color below");
            dialog->exec();
            emit drawButtonClicked();
        }
        else if(drawClicked){
            image.save(filePath, "JPG");
            applyBorder(filePath);
            drawClicked = false;
            numberOfNotDrawnColors = 0;
            colorsIndex = 0;
            ColorDialog *dialog = new ColorDialog(QColor(Qt::black), "Please attach the pen in the color below");
            dialog->exec();
            emit drawButtonClicked();
        }
    }
}

void ImageUploader::setColors(const QList<QString>& colorArr) {
    this->colors = colorArr;

    qDebug() << "IMAGE UPLOADER Colors:";
    for(int i = 0; i < 4; i++){
        qDebug() << this->colors[i];
    }
}

QList<QString> ImageUploader::getColors(){
    return colors;
}
