#ifndef EXAMPLESAPP_H
#define EXAMPLESAPP_H

#include <QWidget>
#include <QVBoxLayout>
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
signals:
    void drawButtonClicked();
};

#endif // EXAMPLESAPP_H
