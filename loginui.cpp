#include "loginui.h"
#include "ui_loginui.h"
#include <QDebug>
LoginUI::LoginUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginUI)
{
    ui->setupUi(this);
}

LoginUI::~LoginUI()
{
    delete ui;
}

void LoginUI::on_pushButtonLogin_clicked()
{
    qDebug()<<"loginUI";
    emit login(ui->lineEdituserName->text().trimmed(),
               ui->lineEditPassword->text().trimmed());
    ui->pushButtonLogin->setDisabled(true);
}
void LoginUI::loginFail()
{
    qDebug()<<"LoginUR::loginFail";
    ui->pushButtonLogin->setEnabled(true);
}
