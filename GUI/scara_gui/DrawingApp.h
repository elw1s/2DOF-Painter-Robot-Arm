#ifndef DRAWINGAPP_H
#define DRAWINGAPP_H

#pragma once

#include <QWidget>
#include <QPushButton>  // Include QPushButton header
#include <QFileDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include "DrawingArea.h"  // Include DrawingArea header
#include "ImagePathsConfig.h"

//class DrawingArea;
//class QPushButton;

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
private:
    DrawingArea *drawingArea;
    QPushButton *saveButton;
    QComboBox *penStrokeDropdown;
    QPushButton *penButton;
    QPushButton *eraserButton;
    QPushButton *resetButton;
};


#endif // DRAWINGAPP_H
