//#include "CSVPlotter.h"
//#include "qwidget.h"
//#include <QApplication>
//#include <QMainWindow>
//#include <QWidget>
//#include <QFrame>
//#include <QPushButton>
//#include <QHBoxLayout>
//#include <QMenuBar>
//#include <QStatusBar>

//using namespace std;

//CSVPlotter::CSVPlotter(QWidget *parent) : QWidget(parent)
//{
//    // Create the horizontal frame
//    QFrame *horizontalFrame = new QFrame(this);
//    horizontalFrame->setGeometry(20, 0, 600, 450);
//    QHBoxLayout *horizontalLayout = new QHBoxLayout(horizontalFrame);

//    // Create the Save Button
//    QPushButton *saveImageButton = new QPushButton("Save the chart as PNG", this);
//    saveImageButton->setGeometry(100, 460, 220, 41);

//    QPushButton *selectFileButton = new QPushButton("Choose File", this);
//    selectFileButton->setGeometry(360,460,180,41);

//    // Create the menu bar
//    QMenuBar *menubar = new QMenuBar(this);

//    // Create the status bar
//    QStatusBar *statusbar = new QStatusBar(this);

//    errorLabel = new QLabel(this);
//    errorLabel->setText("Invalid data in CSV file");
//    errorLabel->setStyleSheet("color: red; font-weight: bold; font-size: 16px;");
//    errorLabel->setAlignment(Qt::AlignCenter);
//    errorLabel->setFixedSize(200, 50);
//    errorLabel->move(250, 200); // Adjust the position as needed
//    errorLabel->hide(); // Hide it initially

//    QFile file("/home/ersel/Documents/Qt_projects/build-test04-Desktop_Qt_6_6_0_GCC_64bit-Debug/yourfile.csv");

//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qDebug() << "Could not open file: " << file.errorString();
//        return;
//    }


//    QTextStream in(&file);
//    QLineSeries *series = new QLineSeries();

//    QChartView *chartView = new QChartView(this);
//    chartView->setRenderHint(QPainter::Antialiasing);
//    chartView->setParent(horizontalFrame);
//    chartView->setVisible(true);

//    // Set size policy and adjust the size of the chartView
//    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    chartView->setMinimumSize(600, 450); // Adjust the minimum size as needed

//    double prevX = std::numeric_limits<double>::lowest(); // Initialize with the smallest possible value

//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        QStringList values = line.split(',');

//        if (values.size() == 2) {
//            bool xOk, yOk;
//            double x = values[0].toDouble(&xOk);
//            double y = values[1].toDouble(&yOk);

//            if (xOk && yOk) {
//                if (x >= prevX) {
//                    series->append(x, y);
//                    prevX = x; // Update the previous X value
//                } else {
//                    qDebug() << "Invalid X value in CSV file";
//                    errorLabel->show(); // Show the error label
//                    chartView->setVisible(false); // Hide the chart
//                    break;
//                }
//            } else {
//                qDebug() << "Invalid data format in CSV file";
//                errorLabel->show(); // Show the error label
//                chartView->setVisible(false); // Hide the chart
//                break;
//            }
//        } else {
//            qDebug() << "Incorrect number of values in line";
//            errorLabel->show(); // Show the error label
//            chartView->setVisible(false); // Hide the chart
//            break;
//        }
//    }

//    file.close();

//    QChart *chart = new QChart();
//    chart->legend()->hide();
//    chart->addSeries(series);

//    chartView->setChart(chart); // Set the chart within the existing chartView

//    // Assuming you have a QPushButton pointer for your save button
//    connect(saveImageButton, &QPushButton::clicked, this, &CSVPlotter::saveChartImage);

//}


//void CSVPlotter::saveChartImage() {
//    qDebug() << "Button clicked, capturing and saving chart image.";
//    if (chartView) {
//        QPixmap p = chartView->grab();
//        p.save("chart.png", "PNG");
//    }
//}

