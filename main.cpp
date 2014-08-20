#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "lib/xware.h"
#include "controlbinaryxware.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/qss/qss/stylesheet.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
    MainWindow w;

    w.Login();


    return a.exec();
}
