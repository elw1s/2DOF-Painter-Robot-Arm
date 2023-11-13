#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#pragma once

#include <QWidget>

class DrawingArea : public QWidget {
    Q_OBJECT

public:
    DrawingArea(QWidget *parent = nullptr);

    const QPixmap &getPixmap() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    static void resizeImage(QPixmap *image, const QSize &newSize);

private:
    QPoint lastPoint;
    QPixmap pixmap;
    bool drawing;
};


#endif // DRAWINGAREA_H
