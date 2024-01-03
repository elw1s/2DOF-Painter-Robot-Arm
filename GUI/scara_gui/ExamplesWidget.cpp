#include "ExamplesWidget.h"
#include <QGridLayout>
#include <QFileDialog>
#include <QDebug>

ExamplesWidget::ExamplesWidget(QWidget *parent) : QWidget(parent) {
    // Example image file paths (you can replace these with your image paths)
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QStringList imageFilePaths = {
        ":/resources/examples/prague.jpg",
        ":/resources/examples/cat.jpeg",
        ":/resources/examples/infinity.jpg",
        ":/resources/examples/africa.jpg",
        ":/resources/examples/lego.jpg",
        ":/resources/examples/europa.jpg",
        ":/resources/examples/gtu.jpg",
        ":/resources/examples/panda.jpg",
        ":/resources/examples/eagle.jpeg",
        ":/resources/examples/istanbul.jpg",
        // Add more image paths as needed
    };

    QScrollArea* scrollArea = new QScrollArea(this);
    QWidget* scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);

    QString scrollBarStyleSheet = "QScrollBar:vertical {"
                                  "    border: 1px solid #19749B;"
                                  "    background: #D9D9D9;"
                                  "    width: 10px; /* adjust as needed */"
                                  "    margin: 0px 0px 0px 0px;"
                                  "}"
                                  "QScrollBar::handle:vertical {"
                                  "    background: #19749B;"
                                  "    min-height: 20px;"
                                  "}"
                                  "QScrollBar::add-line:vertical {"
                                  "    border: 1px solid #19749B;"
                                  "    background: #D9D9D9;"
                                  "    height: 0px;"
                                  "    subcontrol-position: bottom;"
                                  "    subcontrol-origin: margin;"
                                  "}"
                                  "QScrollBar::sub-line:vertical {"
                                  "    border: 1px solid #19749B;"
                                  "    background: #D9D9D9;"
                                  "    height: 0px;"
                                  "    subcontrol-position: top;"
                                  "    subcontrol-origin: margin;"
                                  "}";

    scrollArea->verticalScrollBar()->setStyleSheet(scrollBarStyleSheet);

    // Set the background color for the scroll area
    //scrollWidget->setStyleSheet("background-color: lightgray;"); // Change the background color here

    QGridLayout *gridLayout = new QGridLayout(scrollWidget);
    //gridLayout->setSpacing(1);

    gridLayout->setContentsMargins(5, 5, 5, 5); // Adjust margins for spacing
    gridLayout->setVerticalSpacing(30); // Set vertical spacing between buttons

    int row = 0;
    int col = 0;

    // Create square buttons for each image
    for (const QString &filePath : imageFilePaths) {
        QPushButton *button = createImageButton(filePath);
        gridLayout->addWidget(button, row, col);

        col++;
        if (col == 4) { // Limit 4 buttons per row
            col = 0;
            row++;
        }
    }

    //scrollWidget->setLayout(gridLayout); // Set layout for the scroll widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea); // Add the scroll area to the main layout
    setLayout(mainLayout);
    //setLayout(gridLayout);
}

QPushButton* ExamplesWidget::createImageButton(const QString &filePath) {
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(300, 300); // Square button size

    // Apply border to the button
    button->setStyleSheet("QPushButton { border: 2px solid white;}");

    QLabel *imageLabel = new QLabel(button);
    imageLabel->setScaledContents(true);
    imageLabel->setGeometry(5, 5, 290, 290); // Leave 5px space for the image
    imageLabel->setAlignment(Qt::AlignCenter);

    // Set white background for the QLabel
    imageLabel->setStyleSheet("background-color: white;");

    QPixmap pixmap(filePath);
    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(pixmap);
    }

    button->setProperty("FilePath", QVariant(filePath));
    connect(button, &QPushButton::clicked, this, &ExamplesWidget::imageButtonClicked);

    return button;
}

void ExamplesWidget::imageButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        QVariant filePathVariant = clickedButton->property("FilePath");
        if (filePathVariant.isValid()) {
            QString filePath = filePathVariant.toString();
            if (clickedButton == selectedButton) { // Toggle selection state
                qDebug() << "Unselected image file path:" << filePath;
                updateSelection(nullptr); // Deselect the button
            } else {
                qDebug() << "Selected image file path:" << filePath;
                updateSelection(clickedButton);
            }
        } else {
            qDebug() << "No file path associated with the button.";
        }
    }
}

QString ExamplesWidget::getFilePath(){
    if(this->selectedButton != nullptr){
        QVariant filePathVariant = this->selectedButton->property("FilePath");
        if(filePathVariant.isValid()){
            QString filePath = filePathVariant.toString();
            return filePath;
        }
    }
    return QString();
}

void ExamplesWidget::updateSelection(QPushButton *selectedButton) {
    if (this->selectedButton != nullptr) {
        this->selectedButton->setStyleSheet("QPushButton { border: 2px solid white; }");
        this->selectedButton = nullptr; // Clear selectedButton when deselecting
    }
    if (selectedButton != nullptr) {
        this->selectedButton = selectedButton;
        selectedButton->setStyleSheet("QPushButton { border: 2px solid #33C2FF; }"); // Change border color here
    }
}
