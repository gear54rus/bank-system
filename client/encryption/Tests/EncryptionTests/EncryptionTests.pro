#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T20:48:13
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_encryptiontests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_encryptiontests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -lcryptopp562
