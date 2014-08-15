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
