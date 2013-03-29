#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T19:14:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Part3
TEMPLATE = app


SOURCES += main.cpp\
        part3window.cpp \
    simulation.cpp \
    options.cpp \
    gpant.cpp

HEADERS  += part3window.h \
    simulation.h \
    options.h \
    gpant.h

FORMS    += part3window.ui

include(../common/common.pri)
include(nodes/nodes.pri)

INCLUDEPATH += ../ \
    ../common \
    nodes

OTHER_FILES += \
    nodes/nodes.pri
