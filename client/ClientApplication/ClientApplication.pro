#-------------------------------------------------
#
# Project created by QtCreator 2014-06-08T22:06:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientApplication
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../Core/connection.cpp \
    ../encryption/aescryptor.cpp \
    ../encryption/dhcryptor.cpp \
    ../encryption/rsacryptor.cpp \
    ../encryption/shahasher.cpp

HEADERS  += mainwindow.h \
    ../Core/connection.h \
    ../encryption/aescryptor.h \
    ../encryption/dhcryptor.h \
    ../encryption/rsacryptor.h \
    ../encryption/shahasher.h

FORMS    += mainwindow.ui

LIBS += -lcryptopp562
