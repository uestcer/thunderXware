#include "bind.h"
#include "ui_bind.h"

QString Bind::getActiveCode()
{
    return ui->activeCode->text();
}

void Bind::setActiveCode(QString activeCode)
{
    ui->activeCode->setText(activeCode);
}

Bind::Bind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bind)
{
    ui->setupUi(this);
}

Bind::~Bind()
{
    delete ui;
}

//添加下载器
void Bind::on_buttonAdd_clicked()
{
    QString activeCode = ui->activeCode->text().trimmed();
    if(activeCode == "") {
        emit noActiveCodeSignal();
    }else {
        emit bindSignal(activeCode);
    }

}
//取消
void Bind::on_buttonCancel_clicked()
{
    this->close();
}
