#include "RobotProjectionWidget.h"

RobotProjectionWidget::RobotProjectionWidget(QWidget *parent)
    : QWidget(parent), scaleFactor(1.0), scaleIncrement(0.1),
    isPanning(false), minScaleFactor(0.2) {
    setFocusPolicy(Qt::WheelFocus); // Ensure the widget can receive focus for wheel events
    setMouseTracking(true); // Enable mouse move events even without clicking
}

void RobotProjectionWidget::setAllLines(const QJsonArray &jsonArray) {
    qDebug() << "inside setAllLines appended";
    for (const QJsonValue &arrayValue : jsonArray) {
        if (arrayValue.isArray()) {
            QJsonArray innerArray = arrayValue.toArray();
            qDebug() << innerArray;
            parseJson(innerArray, false);
        }
    }
    updateScaleFactor();
    update(); // Trigger a repaint after loading new drawnLines
}

void RobotProjectionWidget::clear(){
    drawnLines.clear();
    allLines.clear();
    qDebug() << "DrawnLines and allLines are cleared...";
    qDebug() << "Sizes: " << drawnLines.size() << "," << allLines.size();
    updateScaleFactor();
    update();
}

void RobotProjectionWidget::loadLinesFromJson(const QJsonArray &jsonArray) {
    parseJson(jsonArray, true);
    updateScaleFactor();
    update(); // Trigger a repaint after loading new drawnLines
}

void RobotProjectionWidget::parseJson(const QJsonArray &jsonArray, bool isDrawn) {
    //drawnLines.clear(); // Clear any existing drawnLines
    QVector<QPoint> temp;
    for (const auto &lineArray : jsonArray) {
        //QVector<QPoint> linePoints;
       // for (const auto &pointArray : lineArray.toArray()) {
           // int x = pointArray.toArray().at(0).toInt();
           // int y = pointArray.toArray().at(1).toInt();
            int x = lineArray.toArray().at(0).toInt();
            int y = lineArray.toArray().at(1).toInt();
            temp.append(QPoint(x,y));
            //linePoints.append(QPoint(x, y));
        //}
        //drawnLines.append(linePoints);
            if(isDrawn){
                drawnLines.append(temp);
            }
            else{
                allLines.append(temp);
            }
    }
}

void RobotProjectionWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#D9D9D9"));
    painter.setRenderHint(QPainter::Antialiasing);

    painter.scale(scaleFactor, scaleFactor);


    // Apply drawing offset
    painter.translate(drawingOffset);

    painter.setPen(QColor(Qt::black));
    for (const auto &linePoints : allLines) {
            if (linePoints.size() > 1) {
                painter.drawPolyline(linePoints.data(), linePoints.size());
            }
    }

    painter.setPen(QColor(Qt::red));
    for (const auto &linePoints : drawnLines) {
        if (linePoints.size() > 1) {
            painter.drawPolyline(linePoints.data(), linePoints.size());
        }
    }

    //if(drawnLines.size() > 1){
    //        painter.drawPolyline(drawnLines.back().data(), drawnLines.back().size());
    //}
}

void RobotProjectionWidget::wheelEvent(QWheelEvent *event) {
    QPoint angleDelta = event->angleDelta();

    if (!angleDelta.isNull()) {
        int numDegrees = angleDelta.y() / 8;
        numSteps = numDegrees / 15.0;

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
