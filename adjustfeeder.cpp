#include "adjustfeeder.h"
#include "ui_adjustfeeder.h"

AdjustFeeder::AdjustFeeder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdjustFeeder)
{
    ui->setupUi(this);
}

AdjustFeeder::~AdjustFeeder()
{
    delete ui;
}
