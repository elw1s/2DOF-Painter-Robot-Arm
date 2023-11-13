#include "ImageUploader.h"

#include "CropFeature.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsView>

ImageUploader::ImageUploader(QWidget* parent) :
    BaseClass(parent)
{
    m_gridLayout = new QGridLayout(this);
    m_graphicsView = new QGraphicsView(this);
    m_gridLayout->addWidget(m_graphicsView), 0, 0;
    m_pushButton = new QPushButton("Add file", this);
    m_clippedLabel = new QLabel(this);
    m_gridLayout->addWidget(m_clippedLabel, 0, 1);
    m_gridLayout->addWidget(m_pushButton, 1, 0);
    m_clipScene = new CropFeature(this);
    m_graphicsView->setScene(m_clipScene);

    // Connections to slots for adding an image to an application and for adding an image to a label
    connect(m_pushButton, &QPushButton::clicked, this, &ImageUploader::onAddFile);
    connect(m_clipScene, &CropFeature::clippedImage, this, &ImageUploader::onClippedImage);

    resize(640, 480);
}

void ImageUploader::onAddFile()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg)"));
    m_clipScene->setImage(imagePath);
}

void ImageUploader::onClippedImage(const QPixmap& pixmap)
{
    m_clipScene->setImage(pixmap);
    //m_clippedLabel->setPixmap(pixmap);
}
