#ifndef LOGINUI_H
#define LOGINUI_H

#include <QDialog>

namespace Ui {
class LoginUI;
}

class LoginUI : public QDialog
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = 0);
    ~LoginUI();

private:
    Ui::LoginUI *ui;
};

#endif // LOGINUI_H
