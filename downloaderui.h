#ifndef DOWNLOADERUI_H
#define DOWNLOADERUI_H

#include <QWidget>

namespace Ui {
class DownLoaderUI;
}

class DownLoaderUI : public QWidget
{
    Q_OBJECT

public:
    explicit DownLoaderUI(QWidget *parent = 0);
    ~DownLoaderUI();

private:
    Ui::DownLoaderUI *ui;
};

#endif // DOWNLOADERUI_H
