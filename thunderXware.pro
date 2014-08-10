#-------------------------------------------------
#
# Project created by QtCreator 2014-08-10T14:33:51
#
#-------------------------------------------------

QT       += core gui
QT       +=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thunderXware
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    lib/cookieutil.cpp \
    lib/xware.cpp

HEADERS  += mainwindow.h \
    lib/xware_type.h \
    settingdialog.h \
    lib/cookieutil.h \
    lib/xware.h

FORMS    += mainwindow.ui
