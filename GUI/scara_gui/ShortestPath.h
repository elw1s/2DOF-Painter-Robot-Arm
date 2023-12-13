#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QDir>

#include "ShortestPathWidget.h"

class ShortestPath : public QWidget {
    Q_OBJECT

public:
    ShortestPath(QWidget *parent = nullptr);

private:
    ShortestPathWidget *shortestPathWidget;
    QPushButton *solveButton;
    QPushButton *resetButton;
    QGridLayout *numberGrid;
    QHBoxLayout *buttonsAndWidgetLayout;
    void toggleOverlay(bool showOverlay);

public slots:
    void robotDrawingSignal(const bool status);
private slots:
    void onNumberButtonClicked();
    void onSolveButtonClicked();
signals:
    void drawButtonClicked();
};

#endif // SHORTESTPATH_H







