#include "downloaderui.h"
#include "ui_downloaderui.h"

DownLoaderUI::DownLoaderUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownLoaderUI)
{
    ui->setupUi(this);
}

DownLoaderUI::~DownLoaderUI()
{
    delete ui;
}
