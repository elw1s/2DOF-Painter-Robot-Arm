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
    Q_OBJECT

public:
    ImageUploader(QWidget* parent = nullptr);

private slots:
    void onAddFile();
    void onClippedImage(const QPixmap& pixmap);
    void onCropButtonClicked();
    void onImageDropped();
private:
    QPushButton* m_cropButton;
    QGridLayout* m_gridLayout;
    QPushButton* m_pushButton;
    QGraphicsView* m_graphicsView;
    QLabel* m_clippedLabel;
    CropFeature* m_clipScene;
    DragDropScene* m_dragdropScene;
};
