#-------------------------------------------------
#
# Project created by QtCreator 2016-12-28T15:33:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OSLD
TEMPLATE = app

unix:!mac {
    LIBS += -Wl,-rpath=\\\$$ORIGIN/libs
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        osldisplay.cpp \
    block.cpp \
    gate.cpp \
    osldgraphicsengine.cpp \
    descriptionfilereader.cpp \
    diagramitem.cpp \
    connector.cpp \
    subdiagram.cpp \
    rootitempathscene.cpp

HEADERS  += osldisplay.h \
    block.h \
    gate.h \
    osldgraphicsengine.h \
    descriptionfilereader.h \
    diagramitem.h \
    connector.h \
    subdiagram.h \
    rootitempathscene.h

FORMS    += osldisplay.ui
