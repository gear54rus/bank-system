#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T21:53:07
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = encryption
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    rsacryptor.cpp \
    aescryptor.cpp \
    integerencoder.cpp \
    dhcryptor.cpp \
    shahasher.cpp

HEADERS += \
    rsacryptor.h \
    aescryptor.h \
    integerencoder.h \
    dhcryptor.h \
    shahasher.h

LIBS += -lcryptopp562
CONFIG+ = console
