#include "conveyorcalculation.h"
#include "ui_conveyorcalculation.h"

ConveyorCalculation::ConveyorCalculation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConveyorCalculation)
{
    ui->setupUi(this);
}

ConveyorCalculation::~ConveyorCalculation()
{
    delete ui;
}
