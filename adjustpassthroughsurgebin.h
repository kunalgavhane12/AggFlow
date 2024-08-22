#ifndef ADJUSTPASSTHROUGHSURGEBIN_H
#define ADJUSTPASSTHROUGHSURGEBIN_H

#include <QDialog>

namespace Ui {
class AdjustPassThroughSurgeBin;
}

class AdjustPassThroughSurgeBin : public QDialog
{
    Q_OBJECT

public:
    explicit AdjustPassThroughSurgeBin(QWidget *parent = nullptr);
    ~AdjustPassThroughSurgeBin();

private:
    Ui::AdjustPassThroughSurgeBin *ui;
};

#endif // ADJUSTPASSTHROUGHSURGEBIN_H
