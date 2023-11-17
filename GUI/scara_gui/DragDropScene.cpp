#include "DragDropScene.h"

DragDropScene::DragDropScene(QObject *parent) : QGraphicsScene(parent) {
    // Create QGraphicsTextItem for the text
    QGraphicsTextItem *textItem = addText("Drag & Drop Image");
    QFont font = textItem->font();
    font.setPointSize(18); // Set the font size as needed
    textItem->setFont(font);

    // Create QGraphicsPixmapItem for the icon
    QPixmap iconPixmap(QString::fromStdString(DRAG_DROP_ICON)); // Replace with the path to your icon
    QGraphicsPixmapItem *iconItem = addPixmap(iconPixmap);
    iconItem->setPos(-iconPixmap.width() / 2, -iconPixmap.height() / 2);

    // Position the text below the icon
    textItem->setPos(-textItem->boundingRect().width() / 2, iconPixmap.height() / 2);

    // Set the scene size as needed
    setSceneRect(-250, -250, 500, 500); // Adjust the scene rectangle according to your requirements
}

void DragDropScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void DragDropScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void DragDropScene::dropEvent(QGraphicsSceneDragDropEvent *event) {
    qDebug() << "before";
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()){
        qDebug() << "urls are empty";
        return;
    }
    qDebug() << "after";
    QString filePath = urls.first().toLocalFile();
    qDebug() << filePath;
    // Check file extension and load image if it's a supported format (jpg, jpeg, png)
    if (filePath.toLower().endsWith(".jpg") || filePath.toLower().endsWith(".jpeg") || filePath.toLower().endsWith(".png")) {
        // Load image to display in your QGraphicsScene
        qDebug() << "LOADED";
        QImage image(filePath);
        if (!image.isNull()) {
            clear(); // Clear the current scene
            m_droppedPixmap = QPixmap::fromImage(image); // Store the dropped pixmap
            //addPixmap(m_droppedPixmap); // Add image pixmap to the scene
            emit imageDropped(); // Emit the imageDropped signal when an image is dropped
        }
    }
}

QPixmap DragDropScene::getDroppedPixmap() const {
    return m_droppedPixmap;
}
