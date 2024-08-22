#include "adjustpassthroughsurgebin.h"
#include "ui_adjustpassthroughsurgebin.h"

AdjustPassThroughSurgeBin::AdjustPassThroughSurgeBin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdjustPassThroughSurgeBin)
{
    ui->setupUi(this);
}

AdjustPassThroughSurgeBin::~AdjustPassThroughSurgeBin()
{
    delete ui;
}
