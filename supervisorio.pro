#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T14:31:47
#
#-------------------------------------------------

QT       += core gui
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = supervisorio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    threadControl.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    quanser.h \
    threadControl.h \
    qcustomplot.h

FORMS    += mainwindow.ui

RESOURCES += \
    imagens.qrc
