#include "RobotProjectionWidget.h"

RobotProjectionWidget::RobotProjectionWidget(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pixmap = QPixmap(size()); // Initialize the pixmap with the widget size
    //pixmap.fill(Qt::white); // Fill the pixmap with a white background
}

void RobotProjectionWidget::setPointsData(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file.";
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isArray()) {
        QJsonArray jsonArray = doc.array();
        for (const auto& innerArray : jsonArray) {
            if (innerArray.isArray()) {
                QVector<QJsonArray> points;
                QJsonArray array = innerArray.toArray();
                for (const auto& point : array) {
                    if (point.isArray()) {
                        points.append(point.toArray());
                    }
                }
                pointsData.append(points);
            }
        }
    }

    drawPointsOnPixmap(); // Draw points on the pixmap
    update(); // Trigger a repaint
}

void RobotProjectionWidget::drawPointsOnPixmap() {
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Verify if pixmap is valid before drawing
    if (pixmap.isNull()) {
        qDebug() << "Pixmap is null!";
        return;
    }

    // Draw points on the pixmap
    for (const auto& pointList : pointsData) {
        for (const auto& point : pointList) {
            int x = point[0].toInt();
            int y = point[1].toInt();

            // Ensure the painter is active before drawing
            if (!painter.isActive()) {
                qDebug() << "Painter not active!";
                return;
            }

            painter.drawPoint(x, y);
            qDebug() << "X:" << x << "  Y:" << y;
        }
    }

    update(); // Trigger a repaint after drawing all points
}

void RobotProjectionWidget::resizeEvent(QResizeEvent *event){
    if (width() > pixmap.width() || height() > pixmap.height()) {
        int newWidth = qMax(width() + 128, pixmap.width());
        int newHeight = qMax(height() + 128, pixmap.height());
        resizeImage(&pixmap, QSize(newWidth, newHeight));
        update();
    }

    QWidget::resizeEvent(event);
}

void RobotProjectionWidget::resizeImage(QPixmap *image, const QSize &newSize) {
    if (image->size() == newSize)
        return;

    QPixmap newImage(newSize);
    newImage.fill(Qt::white);
    QPainter painter(&newImage);
    painter.drawPixmap(QPoint(0, 0), *image);
    *image = newImage;
}

void RobotProjectionWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap); // Draw the pixmap onto the widget
}
