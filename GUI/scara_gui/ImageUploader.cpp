#include "ImageUploader.h"

ImageUploader::ImageUploader(QWidget* parent) : BaseClass(parent)
{
    m_gridLayout = new QGridLayout(this);
    m_graphicsView = new QGraphicsView(this);
    m_gridLayout->addWidget(m_graphicsView, 0, 0, 1, 2);  // Span the QGraphicsView over 2 columns
    m_pushButton = new QPushButton("Add file", this);
    m_clippedLabel = new QLabel(this);
    //m_gridLayout->addWidget(m_clippedLabel, 0, 2);
    m_pushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_gridLayout->addWidget(m_pushButton, 1, 0);
    m_cropButton = new QPushButton(this);
    m_cropButton->setIcon(QIcon(QString::fromStdString(CROP_ICON)));  // Set the path to your icon
    m_cropButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_gridLayout->addWidget(m_cropButton, 1, 1);

    m_clipScene = new CropFeature(this);
    m_graphicsView->setScene(m_clipScene);

    // Connections
    connect(m_pushButton, &QPushButton::clicked, this, &ImageUploader::onAddFile);
    connect(m_clipScene, &CropFeature::clippedImage, this, &ImageUploader::onClippedImage);
    connect(m_cropButton, &QPushButton::clicked, this, &ImageUploader::onCropButtonClicked);

    resize(640, 480);
}

void ImageUploader::onAddFile()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg *.jpeg)"));
    m_clipScene->setImage(imagePath);
}

void ImageUploader::onClippedImage(const QPixmap& pixmap)
{
    m_clipScene->setImage(pixmap);
}

void ImageUploader::onCropButtonClicked()
{
    m_clipScene->setCropEnabled(!m_clipScene->isCropEnabled());
}
