#ifndef CREATETASKUI_H
#define CREATETASKUI_H

#include <QWidget>
#include <QDebug>
namespace Ui {
class CreateTaskUI;
}

class CreateTaskUI : public QWidget
{
    Q_OBJECT

public:
    explicit CreateTaskUI(QWidget *parent = 0);
    ~CreateTaskUI();

private slots:
    void on_buttonCancel_clicked();

    void on_buttonOK_clicked();

private:
    Ui::CreateTaskUI *ui;
    QString createJson(QString url,QString name);
signals:
    void nullUrlNameSignal();
    void createTaskSignal(QString url,QString taskName);
};

#endif // CREATETASKUI_H
