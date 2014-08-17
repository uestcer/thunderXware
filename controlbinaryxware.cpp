#include "controlbinaryxware.h"
#include "ui_controlbinaryxware.h"
#include <QDebug>
ControlBinaryXware::ControlBinaryXware(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlBinaryXware)
{
    ui->setupUi(this);
    proc_start = new QProcess();
    proc_stop = new QProcess();
    connect(proc_start,&QProcess::readyReadStandardOutput,
            this,&ControlBinaryXware::getXwareStartInfo);
}

ControlBinaryXware::~ControlBinaryXware()
{
    stopXware();//关闭xware

    delete proc_start;
    delete proc_stop;
    delete ui;
}


void ControlBinaryXware::on_buttonStart_clicked()
{

    ui->buttonStart->setText("获取注册码...");
    ui->buttonStart->setDisabled(true);
    startXware();

}
void ControlBinaryXware::startXware(){
    proc_start->start("xware/portal");
}
void ControlBinaryXware::stopXware() {
    QStringList arguments;
    arguments << "-s";
    proc_stop->start("xware/portal",arguments);
    proc_stop->waitForFinished();
}
QString activeCode;
QString bindName;
bool isBinded=false;
void ControlBinaryXware::getXwareStartInfo()
{

    QString str = proc_start->readAll();

    QString bindPrefix = "THIS DEVICE HAS BOUND TO USER: ";
    QString activePrefix = "THE ACTIVE CODE IS: ";
    QString activeCode_tmp;
    QString bindName_tmp;
    int ai=str.indexOf(activePrefix);
    int bi = str.indexOf(bindPrefix);
    if(ai != -1 ) {
        ai += activePrefix.length();
        while(str[ai]!='\n') {
            activeCode_tmp+=str[ai++];
        }
        activeCode=activeCode_tmp;
        isBinded = false;
        emit xwareStartInfoSignal(activeCode_tmp,false);

    }else if(bi != -1) {
        bi +=bindPrefix.length();
        while(str[bi] != '\n') {
            bindName_tmp +=str[bi++];
        }
        bindName_tmp.remove(bindName_tmp.length()-1);
        bindName = bindName_tmp;
        isBinded = true;
        emit xwareStartInfoSignal(bindName_tmp,true);
    }
}


