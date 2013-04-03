#-------------------------------------------------
#
# Project created by QtCreator 2013-03-06T21:43:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Part1
TEMPLATE = app


SOURCES += main.cpp\
        part1window.cpp \
    smartant.cpp

HEADERS  += part1window.h \
    constants.h \
    smartant.h

FORMS    += part1window.ui

include(../common/common.pri)

INCLUDEPATH += ../ \
    ../common

OTHER_FILES += \
    results.txt
