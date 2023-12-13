// ShortestPathWidget.h
#ifndef SHORTESTPATHWIDGET_H
#define SHORTESTPATHWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QFile>
#include <QPainter>

class ShortestPathWidget : public QWidget {
    Q_OBJECT

public:
    ShortestPathWidget(QWidget *parent = nullptr);
    void saveAsImage(const QString &filePath);
    bool solve();
public slots:
    void onCellClicked();
    void onNumberSelected(int number);
    std::pair<int, int> findDestinationPosition();
    std::pair<int, int> findStartPosition();
    void resetGrid();

private:
    QGridLayout *gridLayout;
    QVector<QPushButton *> cellButtons;
    QPoint selectedCell;
    int selectedButtonIndex;
    QPushButton *lastClickedButton;
    QPoint lastClickedPoint;
    int startFlag,destinationFlag;
    std::vector<std::vector<int>> grid;
};

#endif // SHORTESTPATHWIDGET_H
