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
#include "createtaskui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void Login();


    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Xware *xware; //主要是网络网络协协议的封装
    LoginUI *loginUI;//登录
    DownLoaderUI *downLoaderUI;//下载任务

    DownLoaderUI *downLoaderCompleteUI;
    DownLoaderUI *downLoaderTrashUI;
    DownLoaderUI *downLoaderFailUI;
    void initConnectSignal();//大量的connect放在这里
    void refreshTaskButton();
public slots:
    void coreCycle();

private slots:
    void on_buttonDownloading_clicked();
    void on_buttonComplete_clicked();
    void on_buttonTrash_clicked();
    void on_buttonFail_clicked();
};

#endif // MAINWINDOW_H
