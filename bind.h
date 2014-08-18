#ifndef BIND_H
#define BIND_H

#include <QWidget>

namespace Ui {
class Bind;
}

class Bind : public QWidget
{
    Q_OBJECT

public:
    QString getActiveCode();
    void setActiveCode(QString activeCode);
    explicit Bind(QWidget *parent = 0);
    ~Bind();
signals:
    void noActiveCodeSignal();
    void bindSignal(QString activeCode);
private slots:
    void on_buttonAdd_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::Bind *ui;
};

#endif // BIND_H
