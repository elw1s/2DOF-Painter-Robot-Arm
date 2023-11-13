#pragma once

#include <QtWidgets/QWidget>

class QGridLayout;
class QPushButton;
class QGraphicsView;
class QLabel;
class CropFeature;

class ImageUploader : public QWidget
{
    using BaseClass = QWidget;

    Q_OBJECT

public:
    ImageUploader(QWidget* parent = nullptr);

private slots:
    void onAddFile();                           // Slot for adding an image to the application
    void onClippedImage(const QPixmap& pixmap); // Slot for accepting a cropped application area
private:
    QGridLayout* m_gridLayout;
    QPushButton* m_pushButton;
    QGraphicsView* m_graphicsView;
    QLabel* m_clippedLabel;         // A label in which the cropped image will be placed
    CropFeature* m_clipScene;         // The graphical scene in which the image trimming functionality is implemented
};
