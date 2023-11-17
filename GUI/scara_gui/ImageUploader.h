#pragma once

#include <QtWidgets/QWidget>
#include <QGridLayout>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include "CropFeature.h"  // Include the header for CropFeature
#include "ImagePathsConfig.h"
#include "DragDropScene.h"

class ImageUploader : public QWidget
{
    using BaseClass = QWidget;

    Q_OBJECT

public:
    ImageUploader(QWidget* parent = nullptr);

private slots:
    void onAddFile();                           // Slot for adding an image to the application
    void onClippedImage(const QPixmap& pixmap); // Slot for accepting a cropped application area
    void onCropButtonClicked();
    void onImageDropped();
private:
    QPushButton* m_cropButton;
    QGridLayout* m_gridLayout;
    QPushButton* m_pushButton;
    QGraphicsView* m_graphicsView;
    QLabel* m_clippedLabel;         // A label in which the cropped image will be placed
    CropFeature* m_clipScene;         // The graphical scene in which the image trimming functionality is implemented
    DragDropScene* m_dragdropScene;
};
