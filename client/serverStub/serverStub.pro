#-------------------------------------------------
#
# Project created by QtCreator 2014-06-01T01:15:59
#
#-------------------------------------------------

QT       += core network

TARGET = serverStub
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mytcpserver.cpp \
    mytcpclient.cpp


HEADERS += \
    mytcpserver.h \
    mytcpclient.h
