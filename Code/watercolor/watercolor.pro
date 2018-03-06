#-------------------------------------------------
#
# Project created by QtCreator 2018-03-05T13:27:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = watercolor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwindow.cpp \
    splat.cpp \
    manager.cpp \
    brush_type.cpp \
    humidity.cpp

HEADERS  += mainwindow.h \
    openglwindow.h \
    splat.h \
    manager.h \
    brush_type.h \
    humidity.h \
    library.h

FORMS    += mainwindow.ui
