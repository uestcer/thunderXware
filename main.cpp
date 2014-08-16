#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "lib/xware.h"
#include "controlbinaryxware.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.Login();


    return a.exec();
}
