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
    lib/xware.cpp \
    terminal.cpp \
    loginui.cpp \
    downloaderui.cpp

HEADERS  += mainwindow.h \
    lib/xware_type.h \
    settingdialog.h \
    lib/cookieutil.h \
    lib/xware.h \
    terminal.h \
    loginui.h \
    downloaderui.h

FORMS    += mainwindow.ui \
    loginui.ui \
    test.ui \
    downloaderui.ui



OTHER_FILES +=
