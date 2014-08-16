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
    downLoaderCompleteUI = new DownLoaderUI(this);
    downLoaderTrashUI = new DownLoaderUI(this);
    downLoaderFailUI = new DownLoaderUI(this);

    ui->stackedWidget->addWidget(downLoaderUI);
    ui->stackedWidget->addWidget(downLoaderCompleteUI);
    ui->stackedWidget->addWidget(downLoaderTrashUI);
    ui->stackedWidget->addWidget(downLoaderFailUI);

    //放在最后初始化
    initConnectSignal();
}
void MainWindow::initConnectSignal() {
    //接收来自LoginUI的登录click
    connect(loginUI,&LoginUI::login,xware,&Xware::login);
    //登录成功，关联的是coreCycle
    connect(xware,&Xware::loginSuccess,this,&MainWindow::coreCycle);
    //登录失败，关联的是LoginUI的内容
    connect(xware,&Xware::loginFail,loginUI,&LoginUI::loginFail);

    //下载列表显示
    connect(xware,&Xware::listFinished,downLoaderUI,&DownLoaderUI::updateTaskView);
    connect(xware,&Xware::listFinished,downLoaderCompleteUI,
            &DownLoaderUI::updateTaskView);
    connect(xware,&Xware::listFinished,downLoaderTrashUI,
            &DownLoaderUI::updateTaskView);
    connect(xware,&Xware::listFinished,downLoaderFailUI,
            &DownLoaderUI::updateTaskView);
    connect(xware,&Xware::listFinished,this,&MainWindow::refreshTaskButton);

    //下载操作
    connect(downLoaderUI,&DownLoaderUI::add_signal,xware,&Xware::createTask);
    connect(xware,&Xware::createTaskFailSignal,
            downLoaderUI,&DownLoaderUI::createTaskResult);
    connect(downLoaderUI,&DownLoaderUI::start_signal,xware,&Xware::operateTask);
    connect(downLoaderUI,&DownLoaderUI::pause_signal,xware,&Xware::operateTask);
    connect(downLoaderUI,&DownLoaderUI::remove_signal,xware,&Xware::operateTask);

    connect(downLoaderCompleteUI,&DownLoaderUI::add_signal,xware,&Xware::createTask);
    connect(xware,&Xware::createTaskFailSignal,
            downLoaderCompleteUI,&DownLoaderUI::createTaskResult);
    connect(downLoaderCompleteUI,&DownLoaderUI::start_signal,xware,&Xware::operateTask);
    connect(downLoaderCompleteUI,&DownLoaderUI::pause_signal,xware,&Xware::operateTask);
    connect(downLoaderCompleteUI,&DownLoaderUI::remove_signal,xware,&Xware::operateTask);

    connect(downLoaderTrashUI,&DownLoaderUI::add_signal,xware,&Xware::createTask);
    connect(xware,&Xware::createTaskFailSignal,
            downLoaderTrashUI,&DownLoaderUI::createTaskResult);
    connect(downLoaderTrashUI,&DownLoaderUI::start_signal,xware,&Xware::operateTask);
    connect(downLoaderTrashUI,&DownLoaderUI::pause_signal,xware,&Xware::operateTask);
    connect(downLoaderTrashUI,&DownLoaderUI::remove_signal,xware,&Xware::operateTask);

    connect(downLoaderFailUI,&DownLoaderUI::add_signal,xware,&Xware::createTask);
    connect(xware,&Xware::createTaskFailSignal,
            downLoaderFailUI,&DownLoaderUI::createTaskResult);
    connect(downLoaderFailUI,&DownLoaderUI::start_signal,xware,&Xware::operateTask);
    connect(downLoaderFailUI,&DownLoaderUI::pause_signal,xware,&Xware::operateTask);
    connect(downLoaderFailUI,&DownLoaderUI::remove_signal,xware,&Xware::operateTask);


}

void MainWindow::refreshTaskButton()
{
    ui->buttonDownloading->setText("正在下载("
                                       +QString::number(xware->downloadTaskStatus.dlNum)+")");
    ui->buttonComplete->setText("已完成("+
                                   QString::number(xware->downloadTaskStatus.completeNum)+")");
    ui->buttonTrash->setText("垃圾箱("+
                             QString::number(xware->downloadTaskStatus.recycleNum)+")");
    ui->buttonFail->setText("下载失败("+
                            QString::number(xware->downloadTaskStatus.serverFailNum)+")");
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




extern QString listType;
void MainWindow::on_buttonDownloading_clicked()
{


    listType="0";
    xware->list();
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_buttonComplete_clicked()
{

    listType="1";
    xware->list();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_buttonTrash_clicked()
{

    listType="2";
    xware->list();
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_buttonFail_clicked()
{

    listType = "3";
    xware->list();
    ui->stackedWidget->setCurrentIndex(3);
}
