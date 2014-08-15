#include "createtaskui.h"
#include "ui_createtaskui.h"

CreateTaskUI::CreateTaskUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateTaskUI)
{
    ui->setupUi(this);
}

CreateTaskUI::~CreateTaskUI()
{
    delete ui;
}

void CreateTaskUI::on_buttonCancel_clicked()
{

    this->close();
    ui->lineEditName->setText("");
    ui->lineEditUrl->setText("");
}

void CreateTaskUI::on_buttonOK_clicked()
{
    QString url = ui->lineEditUrl->text().trimmed();
    QString taskName = ui->lineEditName->text().trimmed();
    if(url==""||taskName=="") {
        emit nullUrlNameSignal();
        return ;
    }


    //json处理完成，发送信号
    emit createTaskSignal(url,taskName);

    this->close();
    //清空输入框
    ui->lineEditName->setText("");
    ui->lineEditUrl->setText("");

}

QString CreateTaskUI::createJson(QString url, QString name)
{
    /*
     * name:json
     * param: path "C:/TDDOWNLOAD/"
     * tasks:
     * [{
     *  "url":"http://....",
     *  "name":"qq.ext",
     *  "gcid":"",
     *  "cid":"",
     *  "file":0
     * }]
     *
     */
    //手工构造json
    QString str="json={";
    str+="\"path\":\"C:/TDDOWNLOAD/\",";
    str+="\"tasks\":[{\"url\":\""+url+"\",";
    str+="\"name\":\""+name+"\",";
    str+="\"gcid\":\"\",\"cid\":\"\",";
    str+="\"filesize\":0";
    str+="}]}";
    qDebug()<<str;
    return str;

}


