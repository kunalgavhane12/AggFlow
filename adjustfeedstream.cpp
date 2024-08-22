#include "adjustfeedstream.h"
#include "ui_adjustfeedstream.h"

AdjustFeedStream::AdjustFeedStream(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdjustFeedStream)
{
    ui->setupUi(this);
    // Create the QLineSeries
    QLineSeries *series = new QLineSeries();
    series->append(0, 20);
    series->append(20, 40);
    series->append(40, 60);
    series->append(60, 80);
    series->append(80, 100);

    // Create the QChart and add the series
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, 100);
    chart->axes(Qt::Horizontal).first()->setRange(0, 100);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setVisible(true);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->ChartGroupBox->setLayout(layout);

}

AdjustFeedStream::~AdjustFeedStream()
{
    delete ui;
}

//void AdjustFeedStream::on_clear()
//{
//    if (ui->FeedRategroupBox && ui->FeedRategroupBox->layout())
//    {
//        QLayout* layout = ui->FeedRategroupBox->layout();

//        // Iterate over all items in the layout
//        while (QLayoutItem* item = layout->takeAt(0))
//        {
//            // If the item contains a widget, delete the widget
//            if (QWidget* widget = item->widget())
//            {
//                delete widget;
//            }

//            // If the item contains a layout, recursively delete its items
//            if (QLayout* childLayout = item->layout())
//            {
//                // Recursively clear the child layout
//                QLayoutItem* childItem;
//                while ((childItem = childLayout->takeAt(0)) != nullptr)
//                {
//                    if (QWidget* childWidget = childItem->widget())
//                    {
//                        delete childWidget;
//                    }
//                    delete childItem;
//                }
//                delete childLayout;
//            }
//        }
//    }
//}


void AdjustFeedStream::on_clear()
{
    if (ui->FeedRategroupBox && ui->FeedRategroupBox->layout())
    {
        QLayout* layout = ui->FeedRategroupBox->layout();

        // Iterate over all items in the layout
        while (QLayoutItem* item = layout->takeAt(0))
        {
            // If the item contains a widget, delete the widget
            if (QWidget* widget = item->widget())
            {
                delete widget;
            }

            // If the item contains a layout, recursively delete its items
            if (QLayout* childLayout = item->layout())
            {
                // Recursively clear the child layout
                QLayoutItem* childItem;
                while ((childItem = childLayout->takeAt(0)) != nullptr)
                {
                    if (QWidget* childWidget = childItem->widget())
                    {
                        delete childWidget;
                    }
                    delete childItem;
                }
                delete childLayout;
            }
        }
        delete layout;
    }

    // Create the main widget
    QWidget mainWidget;

    // Create a QVBoxLayout for the main widget
    QVBoxLayout* mainLayout = new QVBoxLayout(&mainWidget);

    // Create title and warning labels
    QLabel* titleLabel = new QLabel("Set Material Feed Rate for Each output:");
    titleLabel->setFont(QFont("Arial", 8, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(titleLabel);

    // Create a horizontal layout to contain the title label, warning label, and group box
    QHBoxLayout* horizontalLayout = new QHBoxLayout();

    // Create a QGroupBox
    QGroupBox* groupBox = new QGroupBox("Outputs:");

    // Create a QVBoxLayout for the group box
    QVBoxLayout* groupBoxLayout = new QVBoxLayout(groupBox);
    QSize size(25, 25);

    // First Row
    QWidget* layoutWidget1 = new QWidget();
    QHBoxLayout* hLayout1 = new QHBoxLayout(layoutWidget1);
    QCheckBox* checkBox1 = new QCheckBox("1");
    QLineEdit* lineEdit1 = new QLineEdit("0.0");
    lineEdit1->setMaximumSize(size);
    lineEdit1->setAlignment(Qt::AlignCenter);
    QLabel* label1 = new QLabel("stph");
    hLayout1->addWidget(checkBox1);
    hLayout1->addWidget(lineEdit1);
    hLayout1->addWidget(label1);
    groupBoxLayout->addWidget(layoutWidget1);

    // Second Row
    QWidget* layoutWidget2 = new QWidget();
    QHBoxLayout* hLayout2 = new QHBoxLayout(layoutWidget2);
    QCheckBox* checkBox2 = new QCheckBox("2");
    QLineEdit* lineEdit2 = new QLineEdit("0.0");
    lineEdit2->setMaximumSize(size);
    lineEdit2->setAlignment(Qt::AlignCenter);
    QLabel* label2 = new QLabel("stph");
    hLayout2->addWidget(checkBox2);
    hLayout2->addWidget(lineEdit2);
    hLayout2->addWidget(label2);
    groupBoxLayout->addWidget(layoutWidget2);

    // Third Row
    QWidget* layoutWidget3 = new QWidget();
    QHBoxLayout* hLayout3 = new QHBoxLayout(layoutWidget3);
    QCheckBox* checkBox3 = new QCheckBox("3");
    QLineEdit* lineEdit3 = new QLineEdit("0.0");
    lineEdit3->setMaximumSize(size);
    lineEdit3->setAlignment(Qt::AlignCenter);
    QLabel* label3 = new QLabel("stph");
    hLayout3->addWidget(checkBox3);
    hLayout3->addWidget(lineEdit3);
    hLayout3->addWidget(label3);
    groupBoxLayout->addWidget(layoutWidget3);

    // Fourth Row
    QWidget* layoutWidget4 = new QWidget();
    QHBoxLayout* hLayout4 = new QHBoxLayout(layoutWidget4);
    QCheckBox* checkBox4 = new QCheckBox("4");
    QLineEdit* lineEdit4 = new QLineEdit("0.0");
    lineEdit4->setMaximumSize(size);
    lineEdit4->setAlignment(Qt::AlignCenter);
    QLabel* label4 = new QLabel("stph");
    hLayout4->addWidget(checkBox4);
    hLayout4->addWidget(lineEdit4);
    hLayout4->addWidget(label4);
    groupBoxLayout->addWidget(layoutWidget4);

    // Line Separator
    QFrame* lineSeparator = new QFrame();
    lineSeparator->setFrameShape(QFrame::HLine);
    lineSeparator->setFrameShadow(QFrame::Sunken);
    groupBoxLayout->addWidget(lineSeparator);

//    QLabel* totalLabel = new QLabel("Total:   0.0  stph");
//    totalLabel->setFont(QFont("Arial", 8));
//    groupBoxLayout->addWidget(totalLabel);

    // Set the layout for the group box
    groupBox->setLayout(groupBoxLayout);

    // Add warning label, and group box to the horizontal layout
    QLabel* warningLabel = new QLabel("ATTENTION: If feed contains, Water, adjust the \"Add Water\" tab above.");
    warningLabel->setWordWrap(true);
    horizontalLayout->addWidget(groupBox);
    horizontalLayout->addWidget(warningLabel);

//     Add horizontal layout to the main layout
    mainLayout->addLayout(horizontalLayout);

    ui->FeedRategroupBox->setLayout(mainLayout);
}

