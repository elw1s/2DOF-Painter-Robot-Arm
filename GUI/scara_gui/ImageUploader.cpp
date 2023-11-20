#include "ImageUploader.h"

ImageUploader::ImageUploader(QWidget* parent)
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
    m_graphicsView->setBackgroundBrush(QColor("#D9D9D9"));  // Set the background color
    m_gridLayout->setContentsMargins(25, 75, 75, 20); // Set the margins
    m_clipScene = new CropFeature(this);
    m_dragdropScene = new DragDropScene(this);

    //m_graphicsView->setScene(m_clipScene);
    //m_gridLayout->addWidget(m_dragdrop);
    m_graphicsView->setScene(m_dragdropScene);

    // Connections
    connect(m_pushButton, &QPushButton::clicked, this, &ImageUploader::onAddFile);
    connect(m_clipScene, &CropFeature::clippedImage, this, &ImageUploader::onClippedImage);
    connect(m_cropButton, &QPushButton::clicked, this, &ImageUploader::onCropButtonClicked);
    connect(m_dragdropScene, &DragDropScene::imageDropped, this, &ImageUploader::onImageDropped);

    resize(640, 480);
}

void ImageUploader::onAddFile()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        m_clipScene->setImage(pixmap);
        QGraphicsScene *currentScene = m_graphicsView->scene();
        m_graphicsView->setScene(m_clipScene);
        delete currentScene;
    }
}

void ImageUploader::onClippedImage(const QPixmap& pixmap)
{
    m_clipScene->setImage(pixmap);
}

void ImageUploader::onCropButtonClicked()
{
    m_clipScene->setCropEnabled(!m_clipScene->isCropEnabled());
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
        delete currentScene; // Delete the old scene
        qDebug() << "the other scene is removed!";
    }
}
