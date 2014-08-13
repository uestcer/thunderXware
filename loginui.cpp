#include "loginui.h"
#include "ui_loginui.h"

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
