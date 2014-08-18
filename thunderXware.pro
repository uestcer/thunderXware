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
    downloaderui.cpp \
    tasklistview.cpp \
    createtaskui.cpp \
    controlbinaryxware.cpp \
    bind.cpp

HEADERS  += mainwindow.h \
    lib/xware_type.h \
    settingdialog.h \
    lib/cookieutil.h \
    lib/xware.h \
    terminal.h \
    loginui.h \
    downloaderui.h \
    tasklistview.h \
    createtaskui.h \
    controlbinaryxware.h \
    bind.h

FORMS    += mainwindow.ui \
    loginui.ui \
    test.ui \
    downloaderui.ui \
    createtaskui.ui \
    controlbinaryxware.ui \
    bind.ui



OTHER_FILES += \
    xware/EmbedThunderManager \
    xware/ETMDaemon \
    xware/portal \
    xware/vod_httpserver \
    xware/README
