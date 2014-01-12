#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T15:46:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tiny6410_main
TEMPLATE = app


SOURCES += main.cpp\
        tiny6410dialog.cpp \
    leddialog.cpp \
    buttondialog.cpp \
    pwmdialog.cpp \
    adcdialog.cpp

HEADERS  += tiny6410dialog.h \
    leddialog.h \
    buttondialog.h \
    pwmdialog.h \
    adcdialog.h

FORMS    += tiny6410dialog.ui \
    leddialog.ui \
    buttondialog.ui \
    pwmdialog.ui \
    adcdialog.ui

RESOURCES +=
