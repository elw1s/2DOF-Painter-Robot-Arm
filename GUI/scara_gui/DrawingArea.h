#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#pragma once

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>

class DrawingArea : public QWidget {
    Q_OBJECT

public:
    DrawingArea(QWidget *parent = nullptr);
    void setPenStroke(int stroke);
    void eraserMode(bool mode);
    const QPixmap &getPixmap() const;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &startPoint, const QPoint &endPoint, const QColor &color);
    static void resizeImage(QPixmap *image, const QSize &newSize);

private:
    QPoint lastPoint;
    QPixmap pixmap;
    bool drawing;
    int currentStroke;
    bool eraser;
};


#endif // DRAWINGAREA_H
