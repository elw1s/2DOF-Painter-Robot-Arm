#include "DrawingArea.h"

    DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent), drawing(false) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMouseTracking(true);
        currentStroke = 1;
    }

    const QPixmap& DrawingArea::getPixmap() const {
        return pixmap;
    }

    void DrawingArea::paintEvent(QPaintEvent *event){
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        // Calculate the drawing area excluding the margins
        //QRect drawingRect = rect().adjusted(25, 75, -75, -20);

        // Draw the pixmap within the adjusted area
        //painter.drawPixmap(drawingRect, pixmap, pixmap.rect());
        QPen pen;
        qDebug() << currentStroke;
        pen.setWidth(1000);
        painter.setPen(pen);
        painter.drawPixmap(0, 0, pixmap);
    }

    void DrawingArea::mousePressEvent(QMouseEvent *event){
        lastPoint = event->pos();
        drawing = true;
    }

    void DrawingArea::mouseMoveEvent(QMouseEvent *event){
        if (drawing) {
            QPoint currentPos = event->pos();
            drawLineTo(lastPoint, currentPos);
            lastPoint = currentPos;
        }
    }
    void DrawingArea::mouseReleaseEvent(QMouseEvent *event){
        if (drawing) {
            drawLineTo(lastPoint, event->pos());
            drawing = false;
        }
    }

    void DrawingArea::drawLineTo(const QPoint &startPoint, const QPoint &endPoint) {
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.drawLine(startPoint, endPoint);
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

    void DrawingArea::setPenStroke(int stroke) {
        currentStroke = stroke;
        update(); // Trigger a repaint to apply the new stroke width
    }
