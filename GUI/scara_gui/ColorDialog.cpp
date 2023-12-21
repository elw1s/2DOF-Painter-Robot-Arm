#include "ColorDialog.h"

ColorDialog::ColorDialog(const QColor &initialColor, const QString &labelText, QWidget *parent) : QDialog(parent, Qt::WindowStaysOnTopHint) {
    setupUI();

    // Set initial color for the color square
    colorSquare->setStyleSheet(QString("background-color: %1").arg(initialColor.name()));

    // Set the text for the label
    textLabel->setText(labelText);

    connect(okButton, &QPushButton::clicked, this, &ColorDialog::onOKButtonClicked);
}

void ColorDialog::onOKButtonClicked() {
    accept();
}

void ColorDialog::setupUI() {
    textLabel = new QLabel(this);

    colorSquare = new QLabel(this);
    colorSquare->setFixedSize(50, 50);
    colorSquare->setFrameStyle(QFrame::Box);

    okButton = new QPushButton("Continue", this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(textLabel, 0, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(colorSquare, 1, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(okButton, 2, 0, 1, 2, Qt::AlignCenter);

    setLayout(layout);
    setWindowTitle("Color Selection Dialog");
}
