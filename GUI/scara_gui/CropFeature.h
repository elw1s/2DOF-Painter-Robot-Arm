#pragma once


#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsView>  // Include the header for QGraphicsView
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QMessageBox>

class QGraphicsSceneMouseEvent;
class QGraphicsPixmapItem;
class QGraphicsRectItem;

class CropFeature : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    CropFeature(QObject* parent);
    QPointF previousPosition() const;
    // Method for replacing an image in QGraphicsScene
    void setImage(const QString& filePath);
    void setImage(const QPixmap& pixmap);
    bool isCropEnabled() const;
    void setCropEnabled(bool enabled);
signals:
    void previousPositionChanged(const QPointF previousPosition);
    void clippedImage(const QPixmap& pixmap);  // A signal that transmits the cut out area to the application window to install it in the label

public slots:
    void setPreviousPosition(const QPointF previousPosition);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;


private:
    QGraphicsRectItem* m_selection          {nullptr};
    QGraphicsPixmapItem* m_currentImageItem {nullptr};
    QPointF m_previousPosition;
    bool m_leftMouseButtonPressed           {false};
    bool m_cropEnabled                      {false};
};
