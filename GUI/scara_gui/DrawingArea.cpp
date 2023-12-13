#include "DrawingArea.h"

    DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent), drawing(false) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMouseTracking(true);
        currentStroke = 4;
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
            QPoint currentPos = event->pos();
            if(eraser){
                drawLineTo(lastPoint, currentPos,QColor(255,255,255));

            }
            else if(pen){
                drawLineTo(lastPoint, currentPos,QColor(0,0,0));

            }

            lastPoint = currentPos;
        }
    }
    void DrawingArea::mouseReleaseEvent(QMouseEvent *event){
        if (drawing) {
            if(eraser){
                drawLineTo(lastPoint, event->pos(),QColor(255,255,255));
            }
            else if(pen){
                drawLineTo(lastPoint, event->pos(),QColor(0,0,0));
            }
            drawing = false;
        }
    }

    void DrawingArea::drawLineTo(const QPoint &startPoint, const QPoint &endPoint, const QColor &color) {
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen;
        pen.setWidth(currentStroke);
        pen.setColor(color);
        painter.setPen(pen);
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

    void DrawingArea::eraserMode(bool mode){
        eraser = mode;
    }

    void DrawingArea::penMode(bool mode){
        pen = mode;
    }

    bool DrawingArea::isEraserActive(){
        return eraser;
    }

    bool DrawingArea::isPenActive(){
        return pen;
    }

    void DrawingArea::clearDrawing() {
        pixmap.fill(Qt::white); // Clear the pixmap with white color (or desired background)
        update(); // Update the drawing area
    }
