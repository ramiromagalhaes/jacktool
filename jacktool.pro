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
    extract_patch.cpp

HEADERS  += mainwindow.h \
    PatchExtractorConfiguration.h \
    Exclusion.h \
    extract_patch.h

FORMS    += mainwindow.ui

LIBS += -lopencv_core -lopencv_imgproc
