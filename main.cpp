#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "login.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // MainWindow w;
    //w.show();
    Login *login = new Login;
    login->test();

    return a.exec();
}
