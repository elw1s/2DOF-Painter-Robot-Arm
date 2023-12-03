#include "NumberSelectionDialog.h"


NumberSelectionDialog::NumberSelectionDialog(QWidget *parent) : QDialog(parent) {
    setFixedSize(60, 60); // Set the size of the dialog
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // Set flags
    createButtons();
}

void NumberSelectionDialog::createButtons() {
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);

    QSize buttonSize(20, 20);
    for (int i = 1; i <= 9; ++i) {
        QPushButton *button = new QPushButton(QString::number(i), this);
        connect(button, &QPushButton::clicked, this, &NumberSelectionDialog::onNumberClicked);

        button->setFixedSize(buttonSize);
        button->setStyleSheet("background-color: white;"); // Set white background

        layout->addWidget(button, (i - 1) / 3, (i - 1) % 3);
    }

    layout->setContentsMargins(0, 0, 0, 0); // Remove margins
    layout->setRowStretch(0, 1); // Allow rows and columns to expand
    layout->setColumnStretch(0, 1);

    setLayout(layout);
}

void NumberSelectionDialog::onNumberClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        int selectedNumber = clickedButton->text().toInt();
        emit numberSelected(selectedNumber);
        close();
    }
}
