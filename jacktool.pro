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
    imagemarker.cpp \
    markings.cpp

HEADERS  += mainwindow.h \
    PatchExtractorConfiguration.h \
    extract_patch.h \
    Rectangle.h \
    imagemarker.h \
    markings.h

FORMS    += mainwindow.ui

unix {
    LIBS += -lopencv_core \
            -lopencv_imgproc \
            -lopencv_highgui \
            -lboost_serialization \
            -lboost_filesystem \
            -lboost_system
}

win32 {
    INCLUDEPATH += C:/boost/include/boost-1_54
    LIBS += -LC:/boost/lib \
            -lboost_serialization-mgw46-mt-d-1_54 \
            -lboost_filesystem-mgw46-mt-d-1_54 \
            -lboost_system-mgw46-mt-d-1_54

    INCLUDEPATH += C:/opencv/install/include
    LIBS += -LC:/opencv/lib \
            -lopencv_core \
            -lopencv_imgproc \
            -lopencv_highgui
}
