#ifndef ROBOTPROJECTIONWIDGET_H
#define ROBOTPROJECTIONWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QJsonArray>
#include <QScrollBar>

class RobotProjectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit RobotProjectionWidget(QWidget *parent = nullptr);
    void loadLinesFromJson(const QJsonArray &jsonArray);
    void setAllLines(const QJsonArray &jsonArray);
    void clear();
protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    qreal scaleFactor;
    qreal scaleIncrement;
    QVector<QVector<QPoint>> drawnLines;
    QVector<QVector<QPoint>> allLines;
    //QVector<QPoint> lines;
    QPoint lastPanPos;
    bool isPanning;
    qreal minScaleFactor;
    QPoint drawingOffset;
    qreal numSteps;

    void parseJson(const QJsonArray &jsonArray, bool isDrawn);
    void updateScaleFactor();
};

#endif // ROBOTPROJECTIONWIDGET_H
