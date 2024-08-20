#ifndef ADJUSTFEEDSTREAM_H
#define ADJUSTFEEDSTREAM_H

#include <QDialog>
#include <QtCharts>

namespace Ui {
class AdjustFeedStream;
}

class AdjustFeedStream : public QDialog
{
    Q_OBJECT

public:
    explicit AdjustFeedStream(QWidget *parent = nullptr);
    ~AdjustFeedStream();

private:
    Ui::AdjustFeedStream *ui;
};

#endif // ADJUSTFEEDSTREAM_H
