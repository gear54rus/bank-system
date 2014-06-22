#-------------------------------------------------
#
# Project created by QtCreator 2014-06-06T01:26:05
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = Core
CONFIG   -= app_bundle

CONFIG += console

TEMPLATE = app


SOURCES += main.cpp \
    logger.cpp \
    clientcontroller.cpp \
    ../encryption/aescryptor.cpp \
    ../encryption/dhcryptor.cpp \
    ../encryption/rsacryptor.cpp \
    ../encryption/shahasher.cpp \
    connection.cpp \
    consoleclient.cpp

HEADERS += \
    logger.h \
    clientcontroller.h \
    ../encryption/aescryptor.h \
    ../encryption/dhcryptor.h \
    ../encryption/rsacryptor.h \
    ../encryption/shahasher.h \
    connection.h \
    consoleclient.h

LIBS += -lcryptopp562
