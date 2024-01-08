#ifndef GAZEBOAPP_H
#define GAZEBOAPP_H

#include <QWidget>
#include <QWebEngineView>
#include <QVBoxLayout>

class GazeboApp : public QWidget {
    Q_OBJECT

public:
    GazeboApp(QWidget *parent = nullptr);

    void loadWebPage(const QString& url);

private:
    QVBoxLayout *mainLayout;
    QWebEngineView *webEngineView;
};

#endif // GAZEBOAPP_H
