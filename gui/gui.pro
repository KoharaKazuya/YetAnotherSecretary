#-------------------------------------------------
#
# Project created by QtCreator 2013-09-25T16:25:31
#
#-------------------------------------------------

QT       += core gui webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YetAnotherSecretaryGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    messagebar.cpp \
    apiserver.cpp

HEADERS  += mainwindow.h \
    messagebar.h \
    apiserver.h

FORMS    += mainwindow.ui
