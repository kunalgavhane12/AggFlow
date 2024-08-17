#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include <QWidget>

namespace Ui {
class UserPreferences;
}

class UserPreferences : public QWidget
{
    Q_OBJECT

public:
    explicit UserPreferences(QWidget *parent = nullptr);
    ~UserPreferences();

private:
    Ui::UserPreferences *ui;
};

#endif // USERPREFERENCES_H
