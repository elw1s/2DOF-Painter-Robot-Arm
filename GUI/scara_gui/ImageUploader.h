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
#include "ColorDialog.h"

class ImageUploader : public QWidget
{
    Q_OBJECT

public:
    ImageUploader(QWidget* parent = nullptr);
    QList<QString> getColors();
    bool isSimilarColor(const QColor &color1, const QColor &color2, int threshold = 10);
private slots:
    void onAddFile();
    void onClippedImage(const QPixmap& pixmap);
    void onRotatedImage(const QPixmap& pixmap, qreal rotation, QPointF transformOrigin);
    void onCropButtonClicked();
    void onRotateButtonClicked();
    void onImageDropped();
    void saveImage();
    QPushButton* createStyledButton(const QIcon &icon, const QSize &size, const QString &textColor,
                                    const QString &borderColor, QWidget *parent);
public slots:
    void robotDrawingSignal(const bool status);
    void setColors(const QList<QString>& colorArr);
private:
    QPushButton* cropButton;
    QPushButton* rotateButton;
    QPushButton *saveButton;
    QGridLayout* m_gridLayout;
    QPushButton* addFileButton;
    QGraphicsView* m_graphicsView;
    QLabel* m_clippedLabel;
    CropFeature* m_clipScene;
    DragDropScene* m_dragdropScene;
    QList<QString> colors;
    QImage image;
    int colorsIndex;
    int numberOfNotDrawnColors;
    int getNumberOfColors();
    void applyBorder(const QString filePath);
    QString getNextColor(int start);
    bool drawClicked;
signals:
    void drawButtonClicked();
};
