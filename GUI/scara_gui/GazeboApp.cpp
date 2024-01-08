#include "GazeboApp.h"

GazeboApp::GazeboApp(QWidget *parent) : QWidget(parent) {
    // Create the main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Create the WebEngineView
    webEngineView = new QWebEngineView(this);
    mainLayout->addWidget(webEngineView);
    loadWebPage("http://localhost:5900/vnc_auto.html");
}

void GazeboApp::loadWebPage(const QString& url) {
    // Load the specified URL into the WebEngineView
    if (!url.isEmpty()) {
        webEngineView->load(QUrl(url));
    }
}
