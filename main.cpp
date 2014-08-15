#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "lib/xware.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.Login();
    //w.show();
   // w.test();

     //Xware *login = new Xware;
     //login->test();


    return a.exec();
}
