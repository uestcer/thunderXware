#ifndef DOWNLOADERUI_H
#define DOWNLOADERUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QPushButton>
#include <unistd.h>
#include <QDebug>
namespace Ui {
class DownLoaderUI;
}

class DownLoaderUI : public QWidget
{
    Q_OBJECT

public:
    explicit DownLoaderUI(QWidget *parent = 0);
    void test();
    ~DownLoaderUI();

private:
    Ui::DownLoaderUI *ui;
};

#endif // DOWNLOADERUI_H
