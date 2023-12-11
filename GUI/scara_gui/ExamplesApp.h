#ifndef EXAMPLESAPP_H
#define EXAMPLESAPP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include "ExamplesWidget.h"

class ExamplesApp : public QWidget {
    Q_OBJECT

public:
    ExamplesApp(QWidget *parent = nullptr);
private:
    ExamplesWidget *examplesWidget;
    QPushButton *saveButton;
public slots:
    void robotDrawingSignal(const bool status);
    void saveImage();
signals:
    void drawButtonClicked();
};

#endif // EXAMPLESAPP_H
