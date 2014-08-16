#ifndef CONTROLBINARYXWARE_H
#define CONTROLBINARYXWARE_H
#include <QProcess>
#include <QWidget>

namespace Ui {
class ControlBinaryXware;
}

class ControlBinaryXware : public QWidget
{
    Q_OBJECT

public:
    explicit ControlBinaryXware(QWidget *parent = 0);
    ~ControlBinaryXware();

    void stopXware();
    void startXware();
signals:
    QString xwareStartInfoSignal(QString info,bool hasBind);
public slots:

private slots:


    void on_buttonStart_clicked();

    void getXwareStartInfo();
private:
    Ui::ControlBinaryXware *ui;
     QProcess *proc_start;
     QProcess *proc_stop;
};

#endif // CONTROLBINARYXWARE_H
