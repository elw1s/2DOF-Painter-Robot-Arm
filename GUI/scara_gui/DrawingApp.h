#ifndef DRAWINGAPP_H
#define DRAWINGAPP_H

#pragma once

#include <QWidget>
#include <QPushButton>  // Include QPushButton header
#include <QFileDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include "DrawingArea.h"  // Include DrawingArea header

class DrawingArea;
class QPushButton;

class DrawingApp : public QWidget {
    Q_OBJECT

public:
    DrawingApp(QWidget *parent = nullptr);

private slots:
    void saveImage();
    void setPenStroke(int stroke);

private:
    DrawingArea *drawingArea;
    QPushButton *saveButton;
    QComboBox *penStrokeDropdown;

};


#endif // DRAWINGAPP_H
