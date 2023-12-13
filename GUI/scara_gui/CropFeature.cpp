#include "CropFeature.h"

CropFeature::CropFeature(QObject* parent) : QGraphicsScene(parent), m_leftMouseButtonPressed(false), m_cropEnabled(false), m_currentImageItem(nullptr), m_rotateEnabled(false)
{
}

void CropFeature::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        if (m_cropEnabled)
        {
            // With the left mouse button pressed, remember the position
            m_leftMouseButtonPressed = true;
            setPreviousPosition(event->scenePos());

            // Create a selection square
            m_selection = new QGraphicsRectItem();
            m_selection->setBrush(QBrush(QColor(158, 182, 255, 96)));
            m_selection->setPen(QPen(QColor(158, 182, 255, 200), 1));
            // Add it to the graphic scene
            addItem(m_selection);
        }
        else if(m_rotateEnabled){
            m_leftMouseButtonPressed = true;
            setPreviousPosition(event->scenePos());
        }
        else if (m_currentImageItem && m_currentImageItem->contains(event->scenePos()))
        {
            // Check if the mouse press is within the image bounds
            m_leftMouseButtonPressed = true;
            m_offset = event->scenePos() - m_currentImageItem->pos(); // Calculate the offset from the image's position to the mouse position
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void CropFeature::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_leftMouseButtonPressed && m_cropEnabled)
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
    else if(m_leftMouseButtonPressed && m_rotateEnabled){
        QPointF currentPosition = event->scenePos();
        qreal angle = std::atan2(currentPosition.y() - m_previousPosition.y(), currentPosition.x() - m_previousPosition.x());
        angle = qRadiansToDegrees(angle);

        // To dampen the rotation speed, scale down the angle change
        const qreal rotationScaleFactor = 0.01; // Adjust this factor as needed
        angle *= rotationScaleFactor;

        if (m_currentImageItem)
        {
            m_currentImageItem->setTransformOriginPoint(m_currentImageItem->boundingRect().center());
            m_currentImageItem->setRotation(m_currentImageItem->rotation() + angle);
        }

        setPreviousPosition(currentPosition);
    }
    else if (m_leftMouseButtonPressed && !m_cropEnabled && !m_rotateEnabled && m_currentImageItem)
    {
        // If the left button is pressed and no cropping or rotating is enabled, move the image
        m_currentImageItem->setPos(event->scenePos() - m_offset); // Update the image position based on the mouse movement and the offset
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void CropFeature::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton && m_leftMouseButtonPressed && m_cropEnabled)
    {
        m_leftMouseButtonPressed = false;

        // When releasing the LMB, we form the cut off area
        QRect selectionRect = m_selection->boundingRect().toRect();
        if (m_currentImageItem)
        {
            QPointF sceneCenter = views().first()->mapToScene(views().first()->viewport()->rect().center());
            QPointF imageCenter = m_currentImageItem->mapToScene(m_currentImageItem->boundingRect().center());
            QPointF offset = sceneCenter - imageCenter;

            // Translate the image by the calculated offset to center it
            m_currentImageItem->setPos(m_currentImageItem->pos() + offset);

            clippedImage(m_currentImageItem->pixmap().copy(selectionRect));
            delete m_selection;
        }
        else
        {
            QMessageBox::warning(nullptr, "Warning", "No image loaded!");
            delete m_selection;
        }
    }
    else if(event->button() & Qt::LeftButton && m_leftMouseButtonPressed && m_rotateEnabled){
        rotatedImage(m_currentImageItem->pixmap(), m_currentImageItem->rotation(), m_currentImageItem->transformOriginPoint());
        m_leftMouseButtonPressed = false;
    }
    else if (event->button() & Qt::LeftButton && m_leftMouseButtonPressed && !m_cropEnabled && !m_rotateEnabled)
    {
        // If the left button is released after dragging the image, reset the flag
        m_leftMouseButtonPressed = false;
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void CropFeature::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    QGraphicsView* view = views().first(); // Get the first view

    // Zoom factor
    const qreal scaleFactor = 1.2;
    const QPointF viewCenter = view->mapToScene(view->viewport()->rect().center());
    QPointF mousePoint = event->scenePos();

    // Zoom in or out depending on the mouse wheel motion
    if (event->delta() > 0) {
        view->scale(scaleFactor, scaleFactor);
    } else {
        view->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // Translate the view to keep the mouse position fixed while zooming
    QPointF newViewCenter = view->mapToScene(view->viewport()->rect().center());
    view->translate(newViewCenter.x() - mousePoint.x(), newViewCenter.y() - mousePoint.y());

    QGraphicsScene::wheelEvent(event);
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

void CropFeature::setImage(const QPixmap& pixmap, qreal rotation, QPointF transformOrigin)
{

    if (m_currentImageItem) {
        this->removeItem(m_currentImageItem);
    }

    // Create a new image item with the given pixmap
    m_currentImageItem = new QGraphicsPixmapItem(pixmap);
    addItem(m_currentImageItem);

    // Apply the saved rotation and transform origin to the new image item
    m_currentImageItem->setRotation(rotation);
    m_currentImageItem->setTransformOriginPoint(transformOrigin);
}

void CropFeature::setImage(const QPixmap& pixmap)
{
    // Update the existing image with the new pixmap
    if (m_currentImageItem)
    {
        this->removeItem(m_currentImageItem);
    }

    m_currentImageItem = new QGraphicsPixmapItem(pixmap);
    addItem(m_currentImageItem);
}


bool CropFeature::isCropEnabled() const
{
    return m_cropEnabled;
}

void CropFeature::setCropEnabled(bool enabled)
{
    m_cropEnabled = enabled;
}

bool CropFeature::isRotateEnabled() const{
    return m_rotateEnabled;
}

void CropFeature::setRotateEnabled(bool enabled){
    m_rotateEnabled = enabled;
}

QPixmap CropFeature::getCroppedImage() const {
    if (m_currentImageItem) {
        // Retrieve the transformed pixmap considering the rotation and transformation origin
        const QPixmap& originalPixmap = m_currentImageItem->pixmap();
        QTransform transform;
        transform.translate(m_currentImageItem->transformOriginPoint().x(), m_currentImageItem->transformOriginPoint().y());
        transform.rotate(m_currentImageItem->rotation());
        transform.translate(-m_currentImageItem->transformOriginPoint().x(), -m_currentImageItem->transformOriginPoint().y());

        return originalPixmap.transformed(transform);
    }

    // Return an empty pixmap if there's no image item
    return QPixmap();
}
