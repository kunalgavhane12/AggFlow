#ifndef ADJUSTFEEDER_H
#define ADJUSTFEEDER_H

#include <QDialog>

namespace Ui {
class AdjustFeeder;
}

class AdjustFeeder : public QDialog
{
    Q_OBJECT

public:
    explicit AdjustFeeder(QWidget *parent = nullptr);
    ~AdjustFeeder();

private:
    Ui::AdjustFeeder *ui;
};

#endif // ADJUSTFEEDER_H
