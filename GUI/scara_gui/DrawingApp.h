#ifndef DRAWINGAPP_H
#define DRAWINGAPP_H

#pragma once

#include <QWidget>
#include <QPushButton>  // Include QPushButton header
#include <QFileDialog>
#include <QVBoxLayout>
#include "DrawingArea.h"  // Include DrawingArea header
class DrawingArea;
class QPushButton;

class DrawingApp : public QWidget {
    Q_OBJECT

public:
    DrawingApp(QWidget *parent = nullptr);

private slots:
    void saveImage();

private:
    DrawingArea *drawingArea;
    QPushButton *saveButton;
};


#endif // DRAWINGAPP_H
