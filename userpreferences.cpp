#include "userpreferences.h"
#include "ui_userpreferences.h"

UserPreferences::UserPreferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPreferences)
{
    ui->setupUi(this);
}

UserPreferences::~UserPreferences()
{
    delete ui;
}
