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
    client.cpp \
    clientcontroller.cpp \
    ../../encryption/aescryptor.cpp \
    ../../encryption/dhcryptor.cpp \
    ../../encryption/rsacryptor.cpp \
    ../../encryption/shahasher.cpp \

HEADERS += \
    logger.h \
    client.h \
    clientcontroller.h \
    ../../encryption/aescryptor.h \
    ../../encryption/dhcryptor.h \
    ../../encryption/rsacryptor.h \
    ../../encryption/shahasher.h \

LIBS += -lcryptopp562
