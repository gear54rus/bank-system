#-------------------------------------------------
#
# Project created by QtCreator 2014-05-31T21:27:13
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        loginwindow.cpp \
    settingsform.cpp \
    accountform.cpp \
    client.cpp \
    account.cpp \
    message.cpp

HEADERS  += loginwindow.h \
    settingsform.h \
    accountform.h \
    client.h \
    account.h \
    message.h

FORMS    += loginwindow.ui \
    settingsform.ui \
    accountform.ui
