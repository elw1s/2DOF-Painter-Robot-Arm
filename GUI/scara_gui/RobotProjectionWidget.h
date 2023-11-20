#ifndef ROBOTPROJECTIONWIDGET_H
#define ROBOTPROJECTIONWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QPainter>
#include <QFile>
#include <QJsonDocument>
#include <QPixmap>
#include <QResizeEvent>

class RobotProjectionWidget : public QWidget {
    Q_OBJECT

public:
    RobotProjectionWidget(QWidget *parent = nullptr);
    void setPointsData(const QString& filePath);
    void drawPointsOnPixmap();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    QVector<QVector<QJsonArray>> pointsData;
    QPixmap pixmap;
    static void resizeImage(QPixmap *image, const QSize &newSize);
};
#endif // ROBOTPROJECTIONWIDGET_H
