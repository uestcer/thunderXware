#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    xware = new Xware(this);
    loginUI = new LoginUI(this);
    connect(xware,&Xware::hasDownloader,
            this,&MainWindow::addDownloader);

    connect(ui->cmbDownloaders, SIGNAL(activated(int)),
            ui->stackedWidget, SLOT(setCurrentIndex(int)));
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
    //接收来自LoginUI的登录click
    connect(loginUI,&LoginUI::login,xware,&Xware::login);
    //登录成功，关联的是coreCycle
    connect(xware,&Xware::loginSuccess,this,&MainWindow::coreCycle);
    //登录失败，关联的是LoginUI的内容
    connect(xware,&Xware::loginFail,loginUI,&LoginUI::loginFail);
    loginUI->show();

}
void MainWindow::coreCycle() {
    qDebug()<<"in coreCycle";
    loginUI->hide();
    this->show();

}


void MainWindow::addDownloader(QList<PeerList>& newDownloaders) {
    QList<PeerList> downloaders = xware->getPeerList();//原来的
    if(downloaders.size() == 0) {
        downloaders.append(newDownloaders);
        xware->setPeerList(newDownloaders);
    }

    //进行比校，决定是否更新




}

void MainWindow::test() {
    DownLoaderUI *widget = new DownLoaderUI;
   // ui->stackedWidget->addWidget(widget);
    //listFinished(DownloaderTaskStatus &);
    connect(xware,&Xware::listFinished,widget,&DownLoaderUI::updateTaskView);
   // widget->test();
    widget->show();
}
