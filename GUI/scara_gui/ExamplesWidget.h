#ifndef EXAMPLESWIDGET_H
#define EXAMPLESWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QScrollBar>

class ExamplesWidget : public QWidget {
    Q_OBJECT
public:
    explicit ExamplesWidget(QWidget *parent = nullptr);
    QString getFilePath();

private slots:
    void imageButtonClicked();

private:
    QPushButton* createImageButton(const QString &filePath);
    void updateSelection(QPushButton *selectedButton);

    QPushButton *selectedButton = nullptr;
};

#endif // EXAMPLESWIDGET_H
