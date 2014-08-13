#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QStackedWidget>
#include "lib/xware.h"
#include "loginui.h"
#include "lib/xware_type.h"
#include "downloaderui.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void Login();
    void addDownloader(QList<PeerList>& newDownloaders);

    void test();

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Xware *xware;
    LoginUI *loginUI;
public slots:
    void coreCycle();

};

#endif // MAINWINDOW_H
