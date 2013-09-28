#-------------------------------------------------
#
# Project created by QtCreator 2013-09-26T01:08:15
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = YetAnotherSecretaryProxy
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    yaswebproxy.cpp \
    bodyparserbase.cpp \
    bodyparsermain.cpp \
    httpparser.cpp

HEADERS += \
    yaswebproxy.h \
    bodyparserbase.h \
    bodyparsermain.h \
    httpparser.h
