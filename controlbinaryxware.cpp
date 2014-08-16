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
    delete ui;
    delete proc_start;
    delete proc_stop;
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

}

void ControlBinaryXware::getXwareStartInfo()
{

    QString str = proc_start->readAll();

    QString bindPrefix = "THIS DEVICE HAS BOUND TO USER: ";
    QString activePrefix = "THE ACTIVE CODE IS: ";
    QString activeCode;
    QString bindName;
    int ai=str.indexOf(activePrefix);
    int bi = str.indexOf(bindPrefix);
    if(ai != -1 ) {
        ai += activeCode.length();
        while(str[ai]!='\n') {
            activeCode+=str[ai++];
        }
        qDebug()<<activeCode;
        emit xwareStartInfoSignal(activeCode,false);

    }else if(bi != -1) {
        bi +=bindPrefix.length();
        while(str[bi] != '\n') {
            bindName +=str[bi++];
        }
        bindName.remove(bindName.length()-1);
        qDebug()<<bindName;
        emit xwareStartInfoSignal(bindName,true);
    }
}


