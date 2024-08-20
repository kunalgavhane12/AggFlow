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
