#ifndef DRAWINGAPP_H
#define DRAWINGAPP_H

#pragma once

#include <QWidget>
#include <QPushButton>  // Include QPushButton header
#include <QFileDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCoreApplication>
#include "DrawingArea.h"  // Include DrawingArea header
#include "ImagePathsConfig.h"

class DrawingApp : public QWidget {
    Q_OBJECT

public:
    DrawingApp(QWidget *parent = nullptr);

private slots:
    void saveImage();
    void setPenStroke(int stroke);
    void setEraserTrue();
    void setEraserFalse();
    void resetDrawing();
    QPushButton* createStyledButton(const QIcon &icon, const QSize &size, const QString &textColor,
                                    const QString &borderColor, QWidget *parent);

public slots:
    void robotDrawingSignal(const bool status);
private:
    DrawingArea *drawingArea;
    QPushButton *saveButton;
    QComboBox *penStrokeDropdown;
    QPushButton *penButton;
    QPushButton *eraserButton;
    QPushButton *resetButton;

signals:
    void drawButtonClicked();
};


#endif // DRAWINGAPP_H
