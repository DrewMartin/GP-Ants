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
    entity.cpp \
    food.cpp \
    graphicsview.cpp \
    ant.cpp \
    cell.cpp \
    pheremone.cpp

HEADERS  += part1window.h \
    entity.h \
    food.h \
    graphicsview.h \
    ant.h \
    cell.h \
    pheremone.h \
    constants.h

FORMS    += part1window.ui
