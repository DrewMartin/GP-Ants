#-------------------------------------------------
#
# Project created by QtCreator 2013-03-20T13:47:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Part2
TEMPLATE = app


SOURCES += main.cpp\
        part2window.cpp \
    variablenode.cpp \
    twoargnode.cpp \
    singleargnode.cpp \
    mathnode.cpp \
    constantnode.cpp

HEADERS  += part2window.h \
    variablenode.h \
    twoargnode.h \
    singleargnode.h \
    operators.h \
    mathnode.h \
    constantnode.h

FORMS    += part2window.ui

include(../common/common.pri)

INCLUDEPATH += ../
