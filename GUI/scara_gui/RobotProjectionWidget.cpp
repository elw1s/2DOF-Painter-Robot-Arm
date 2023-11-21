#include "RobotProjectionWidget.h"

RobotProjectionWidget::RobotProjectionWidget(QWidget *parent)
    : QWidget(parent), scaleFactor(1.0), scaleIncrement(0.1),
    isPanning(false), minScaleFactor(0.2) {
    setFocusPolicy(Qt::WheelFocus); // Ensure the widget can receive focus for wheel events
    setMouseTracking(true); // Enable mouse move events even without clicking
}

void RobotProjectionWidget::loadLinesFromJson(const QJsonArray &jsonArray) {
    parseJson(jsonArray);
    updateScaleFactor();
    update(); // Trigger a repaint after loading new lines
}

void RobotProjectionWidget::parseJson(const QJsonArray &jsonArray) {
    lines.clear(); // Clear any existing lines

    for (const auto &lineArray : jsonArray) {
        QVector<QPoint> linePoints;
        for (const auto &pointArray : lineArray.toArray()) {
            int x = pointArray.toArray().at(0).toInt();
            int y = pointArray.toArray().at(1).toInt();
            linePoints.append(QPoint(x, y));
        }
        lines.append(linePoints);
    }
}

void RobotProjectionWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#D9D9D9"));
    painter.setRenderHint(QPainter::Antialiasing);

    painter.scale(scaleFactor, scaleFactor);
    painter.setPen(QColor(Qt::black));

    // Apply drawing offset
    painter.translate(drawingOffset);

    for (const auto &linePoints : lines) {
        if (linePoints.size() > 1) {
            painter.drawPolyline(linePoints.data(), linePoints.size());
        }
    }
}

void RobotProjectionWidget::wheelEvent(QWheelEvent *event) {
    QPoint angleDelta = event->angleDelta();

    if (!angleDelta.isNull()) {
        int numDegrees = angleDelta.y() / 8;
        qreal numSteps = numDegrees / 15.0;

        if (numSteps > 0) {
            scaleFactor *= (1.0 + scaleIncrement);
        } else {
            if (scaleFactor > minScaleFactor) {
                scaleFactor /= (1.0 + scaleIncrement);
            }
        }

        update();
        event->accept();
    }

    QWidget::wheelEvent(event);
}

void RobotProjectionWidget::updateScaleFactor() {
    qreal widthScale = static_cast<qreal>(width()) / static_cast<qreal>(this->width());
    qreal heightScale = static_cast<qreal>(height()) / static_cast<qreal>(this->height());

    scaleFactor = qMin(widthScale, heightScale);
}

void RobotProjectionWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPanPos = event->pos();
        isPanning = true;
        event->accept();
    }
}

void RobotProjectionWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        QPoint delta = event->pos() - lastPanPos;
        lastPanPos = event->pos();

        // Invert the delta for X and Y components
        QPoint invertedDelta = QPoint(-delta.x(), -delta.y());

        // Update the drawing offset using the inverted delta
        drawingOffset += invertedDelta;

        update(); // Trigger a repaint after changing the drawing offset

        event->accept();
    }
}


void RobotProjectionWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isPanning = false;
        event->accept();
    }
}
