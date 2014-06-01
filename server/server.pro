#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T12:07:41
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = server
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    core/core.cpp \
    core/logger.cpp

HEADERS += \
    core/core.h \
    core/logger.h
