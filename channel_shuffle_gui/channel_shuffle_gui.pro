#-------------------------------------------------
#
# Project created by QtCreator 2015-01-16T15:53:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = channel_shuffle_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    droparea.cpp \
    outputarea.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    droparea.h \
    outputarea.h \
    about.h

FORMS    += mainwindow.ui \
    about.ui

DISTFILES +=

RESOURCES += \
    channel_shuffle_gui.qrc
