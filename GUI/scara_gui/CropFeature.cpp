#include "CropFeature.h"

#include <QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>


CropFeature::CropFeature(QObject* parent) : QGraphicsScene(parent)
{

}

void CropFeature::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        // With the left mouse button pressed, remember the position
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());

        // Create a selection square
        m_selection = new QGraphicsRectItem();
        m_selection->setBrush(QBrush(QColor(158,182,255,96)));
        m_selection->setPen(QPen(QColor(158,182,255,200),1));
        // Add it to the graphic scene
        addItem(m_selection);
    }

    QGraphicsScene::mousePressEvent(event);
}

void CropFeature::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_leftMouseButtonPressed)
    {
        // Form the selection area when moving with the mouse while pressing the LMB
        auto dx = event->scenePos().x() - m_previousPosition.x();
        auto dy = event->scenePos().y() - m_previousPosition.y();
        auto resultDelta = qMax(qAbs(dx), qAbs(dy));
        m_selection->setRect((dx > 0) ? m_previousPosition.x() : m_previousPosition.x() - resultDelta,
                             (dy > 0) ? m_previousPosition.y() : m_previousPosition.y() - resultDelta,
                             qAbs(resultDelta),
                             qAbs(resultDelta));
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void CropFeature::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        m_leftMouseButtonPressed = false;

        // When releasing the LMB, we form the cut off area
        QRect selectionRect = m_selection->boundingRect().toRect();
        clippedImage(m_currentImageItem->pixmap().copy(selectionRect));
        delete m_selection;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void CropFeature::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(m_previousPosition);
}

QPointF CropFeature::previousPosition() const
{
    return m_previousPosition;
}

void CropFeature::setImage(const QString& filePath)
{
    if (m_currentImageItem)
    {
        this->removeItem(m_currentImageItem);
    }
    m_currentImageItem = new QGraphicsPixmapItem(QPixmap(filePath));
    addItem(m_currentImageItem);
}

void CropFeature::setImage(const QPixmap& pixmap)
{
    // Update the existing image with the new pixmap
    if (m_currentImageItem)
    {
        this->removeItem(m_currentImageItem);
    }

    m_currentImageItem = new QGraphicsPixmapItem(pixmap);

    // Access the QGraphicsView associated with this scene
    QGraphicsView* view = views().first();

    if (view)
    {
        // Scale the pixmap to fit into the QGraphicsView
        QSize viewSize = view->size();
        QPixmap scaledPixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        m_currentImageItem->setPixmap(scaledPixmap);
        addItem(m_currentImageItem);
        // You might need to call this->update() or view->update() to force a redraw
    }
}
