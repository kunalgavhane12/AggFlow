#ifndef CONVEYORCALCULATION_H
#define CONVEYORCALCULATION_H

#include <QDialog>

namespace Ui {
class ConveyorCalculation;
}

class ConveyorCalculation : public QDialog
{
    Q_OBJECT

public:
    explicit ConveyorCalculation(QWidget *parent = nullptr);
    ~ConveyorCalculation();

private:
    Ui::ConveyorCalculation *ui;
};

#endif // CONVEYORCALCULATION_H
