#ifndef CREATETASKUI_H
#define CREATETASKUI_H

#include <QWidget>

namespace Ui {
class CreateTaskUI;
}

class CreateTaskUI : public QWidget
{
    Q_OBJECT

public:
    explicit CreateTaskUI(QWidget *parent = 0);
    ~CreateTaskUI();

private:
    Ui::CreateTaskUI *ui;
};

#endif // CREATETASKUI_H
