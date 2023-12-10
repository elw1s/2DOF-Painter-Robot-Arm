#ifndef XOXWIDGET_H
#define XOXWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QDateTime>
#include <QDir>

struct Move {
    int index;
    int score;
};

class XOXWidget : public QWidget {
    Q_OBJECT

public:
    explicit XOXWidget(QWidget *parent = nullptr);
    void resetGame();

private slots:
    void buttonClicked();

private:
    QGridLayout *gridLayout;
    std::vector<QPushButton*> buttons;
    bool isUserTurn;
    bool isGameOver;
    void checkGameStatus();
    void makeComputerMove();
    int evaluateBoard();
    bool isMoveLegal(int move);
    Move minimax(bool isMaximizing);
    void printBoard();
    void saveMoveImage();
    QPushButton *lastMoveX;
    QPushButton *lastMoveO;

signals:
    void gameOver(const int result); // Add this line to declare the signal
    void drawButtonClicked();
};

#endif // XOXWIDGET_H
