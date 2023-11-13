#include "DrawingArea.h"
#include <QPainter>
#include <QResizeEvent>

    DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent), drawing(false) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMouseTracking(true);
    }

    const QPixmap& DrawingArea::getPixmap() const {
        return pixmap;
    }

    void DrawingArea::paintEvent(QPaintEvent *event){
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.drawPixmap(0, 0, pixmap);
    }

    void DrawingArea::mousePressEvent(QMouseEvent *event){
        lastPoint = event->pos();
        drawing = true;
    }

    void DrawingArea::mouseMoveEvent(QMouseEvent *event){
        if (drawing) {
            drawLineTo(event->pos());
        }
    }

    void DrawingArea::mouseReleaseEvent(QMouseEvent *event){
        if (drawing) {
            drawLineTo(event->pos());
            drawing = false;
        }
    }

    void DrawingArea::drawLineTo(const QPoint &endPoint) {
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.drawLine(lastPoint, endPoint);
        lastPoint = endPoint;

        update();
    }

    void DrawingArea::resizeEvent(QResizeEvent *event){
        if (width() > pixmap.width() || height() > pixmap.height()) {
            int newWidth = qMax(width() + 128, pixmap.width());
            int newHeight = qMax(height() + 128, pixmap.height());
            resizeImage(&pixmap, QSize(newWidth, newHeight));
            update();
        }

        QWidget::resizeEvent(event);
    }

    void DrawingArea::resizeImage(QPixmap *image, const QSize &newSize) {
        if (image->size() == newSize)
            return;

        QPixmap newImage(newSize);
        newImage.fill(Qt::white);
        QPainter painter(&newImage);
        painter.drawPixmap(QPoint(0, 0), *image);
        *image = newImage;
    }
