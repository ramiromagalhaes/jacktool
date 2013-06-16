#-------------------------------------------------
#
# Project created by QtCreator 2013-06-13T19:51:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jacktool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    extract_patch.cpp \
    markertool.cpp

HEADERS  += mainwindow.h \
    PatchExtractorConfiguration.h \
    extract_patch.h \
    Rectangle.h \
    markertool.h

FORMS    += mainwindow.ui

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui
