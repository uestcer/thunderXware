#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    xware = new Xware(this);
    loginUI = new LoginUI(this);

    downLoaderUI= new DownLoaderUI(this);
    ui->stackedWidget->addWidget(downLoaderUI);






    //这个放在最后初始化
    initConnectSignal();
}
void MainWindow::initConnectSignal() {
    //接收来自LoginUI的登录click
    connect(loginUI,&LoginUI::login,xware,&Xware::login);
    //登录成功，关联的是coreCycle
    connect(xware,&Xware::loginSuccess,this,&MainWindow::coreCycle);
    //登录失败，关联的是LoginUI的内容
    connect(xware,&Xware::loginFail,loginUI,&LoginUI::loginFail);

    //与下载显示相关
    connect(xware,&Xware::listFinished,downLoaderUI,&DownLoaderUI::updateTaskView);

    //与各种下载操作相关

    connect(downLoaderUI,&DownLoaderUI::add_signal,xware,&Xware::createTask);
    connect(downLoaderUI,&DownLoaderUI::start_signal,xware,&Xware::operateTask);
    connect(downLoaderUI,&DownLoaderUI::pause_signal,xware,&Xware::operateTask);
    connect(downLoaderUI,&DownLoaderUI::remove_signal,xware,&Xware::operateTask);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete xware;
    delete loginUI;
}
void MainWindow::Login()
{
    qDebug()<<"MainWindow::Login()";

    loginUI->show();

}
void MainWindow::coreCycle() {
    qDebug()<<"in coreCycle";
    loginUI->hide();
    this->show();

}


void MainWindow::addDownloader(QList<PeerList>& newDownloaders) {




}

void MainWindow::test() {


}
