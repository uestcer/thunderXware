#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setMinimumWidth(1000);
    setMinimumHeight(600);
    xware = new Xware(this);
    loginUI = new LoginUI();
    xwareControl = new ControlBinaryXware;

    xwareStartStatus = new QLabel("xware未启动");
    xwareBindStatus = new QLabel("xware未绑定");

    ui->statusBar->addPermanentWidget(xwareStartStatus);
    ui->statusBar->addPermanentWidget(xwareBindStatus);



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
    init();
    settingUI();
}
void MainWindow::settingUI() {

    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->topWidget->setObjectName("topWidget");
    ui->leftWidget->setObjectName("leftWidget");
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

    connect(xwareControl,&ControlBinaryXware::xwareStartInfoSignal,
            this,&MainWindow::checkXwareInfo);

    //添加下载器
    connect(&bindUI,&Bind::bindSignal,this,&MainWindow::bindDownloader);

    //下载器选择
    connect(ui->selectDownloader, SIGNAL(activated(int)),
            this,SLOT(selectDownloader(int)));
    connect(xware,&Xware::hasDownloader,this,&MainWindow::updateDownloader);

}
void MainWindow::init() {
    xwareControl->startXware();
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
    delete xwareControl;
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
extern QString downloaderPID;
void MainWindow::updateDownloader() {
    //qDebug()<<"updateDownLoader";
    QList<PeerList> peers = xware->peerList;
    int size = peers.length();
    ui->selectDownloader->clear();
    for(int i= 0;i<size;i++) {
        ui->selectDownloader->addItem(peers.at(i).name);
        pidIndexMap[peers.at(i).pid]=i;
        indexPidMap[i]=peers.at(i).pid;

    }

    ui->selectDownloader->setCurrentIndex(pidIndexMap[downloaderPID]);
    if(xware->peerList.at(pidIndexMap[downloaderPID]).online) {
        ui->downloaderStatus->setText("在线");
    }else {
        ui->downloaderStatus->setText("离线");
    }
}

void MainWindow::selectDownloader(int index) {

    downloaderPID = indexPidMap[index];
    xware->listPeer();

}

void MainWindow::checkXwareInfo(QString info, bool isBinded)
{
    xwareStartStatus->setText("本机xware已启动");
    if(isBinded) {
        xwareBindStatus->setText(info);
    }else {
        xwareBindStatus->setText(info);
    }
}

void MainWindow::bindDownloader(QString activeCode) {
    qDebug()<<"bindDownloader";
    QString args = "/bind?boxName=&key="+activeCode;
    xware->operateTask(args);
}

void MainWindow::on_pushButton_clicked()
{
    bindUI.setActiveCode("");
    bindUI.show();
}

