#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QMap>
#include "lib/xware.h"
#include "loginui.h"
#include "lib/xware_type.h"
#include "downloaderui.h"
#include "createtaskui.h"
#include "controlbinaryxware.h"
#include "bind.h"
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

    QLabel *xwareStartStatus;//xware是否启动
    QLabel *xwareBindStatus;//xware是否已经绑定

    ControlBinaryXware *xwareControl;
    Bind bindUI;
    void initConnectSignal();//大量的connect放在这里
    void refreshTaskButton();
    void init();//初始化相关工作
    //<pid,index>
    QMap<QString,int> pidIndexMap;
    //<index,pid>
    QMap<int,QString> indexPidMap;

    void settingUI() ;
public slots:
    void coreCycle();

private slots:
    void on_buttonDownloading_clicked();
    void on_buttonComplete_clicked();
    void on_buttonTrash_clicked();
    void on_buttonFail_clicked();
    void checkXwareInfo(QString info, bool isBinded);
    void selectDownloader(int index);
    void updateDownloader();
    void bindDownloader(QString activeCode);
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
