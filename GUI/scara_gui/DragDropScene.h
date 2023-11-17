#ifndef DRAGDROPSCENE_H
#define DRAGDROPSCENE_H

#include <QGraphicsScene>
#include <QMimeData>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QUrl>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include "ImagePathsConfig.h"


class DragDropScene : public QGraphicsScene {
    Q_OBJECT

public:
    DragDropScene(QObject *parent = nullptr);

    // Method to retrieve the dropped pixmap
    QPixmap getDroppedPixmap() const;

signals:
    void imageDropped();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    QPixmap m_droppedPixmap;
};

#endif // DRAGDROPSCENE_H
