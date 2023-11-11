#include <QtWidgets>

class DrawingArea : public QWidget {
public:
    DrawingArea(QWidget *parent = nullptr) : QWidget(parent), drawing(false) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMouseTracking(true);
    }

    const QPixmap& getPixmap() const {
        return pixmap;
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.drawPixmap(0, 0, pixmap);
    }

    void mousePressEvent(QMouseEvent *event) override {
        lastPoint = event->pos();
        drawing = true;
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (drawing) {
            drawLineTo(event->pos());
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (drawing) {
            drawLineTo(event->pos());
            drawing = false;
        }
    }

    void drawLineTo(const QPoint &endPoint) {
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.drawLine(lastPoint, endPoint);
        lastPoint = endPoint;

        update();
    }

    void resizeEvent(QResizeEvent *event) override {
        if (width() > pixmap.width() || height() > pixmap.height()) {
            int newWidth = qMax(width() + 128, pixmap.width());
            int newHeight = qMax(height() + 128, pixmap.height());
            resizeImage(&pixmap, QSize(newWidth, newHeight));
            update();
        }

        QWidget::resizeEvent(event);
    }

    static void resizeImage(QPixmap *image, const QSize &newSize) {
        if (image->size() == newSize)
            return;

        QPixmap newImage(newSize);
        newImage.fill(Qt::white);
        QPainter painter(&newImage);
        painter.drawPixmap(QPoint(0, 0), *image);
        *image = newImage;
    }

private:
    QPoint lastPoint;
    QPixmap pixmap;
    bool drawing;
};

class DrawingApp : public QWidget {
public:
    DrawingApp(QWidget *parent = nullptr) : QWidget(parent) {
        drawingArea = new DrawingArea(this);

        saveButton = new QPushButton("Save Image", this);
        connect(saveButton, &QPushButton::clicked, this, &DrawingApp::saveImage);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(drawingArea);
        layout->addWidget(saveButton);

        setLayout(layout);
    }

private slots:
    void saveImage() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "JPEG Image (*.jpg)");

        if (!fileName.isEmpty()) {
            drawingArea->getPixmap().toImage().save(fileName, "JPG");
        }
    }

private:
    DrawingArea *drawingArea;
    QPushButton *saveButton;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DrawingApp drawingApp;
    drawingApp.setGeometry(100, 100, 800, 600);
    drawingApp.show();

    return app.exec();
}
