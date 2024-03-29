#-------------------------------------------------
#
# Project created by QtCreator 2016-12-28T15:43:58
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Stimulator
TEMPLATE = app

unix:!mac {
    LIBS += -Wl, -rpath=\\\$$ORIGIN/libs
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

INCLUDEPATH += ../OSLD/


SOURCES += main.cpp\
        mainwindow.cpp \
    ../OSLD/block.cpp \
    ../OSLD/connector.cpp \
    ../OSLD/descriptionfilereader.cpp \
    ../OSLD/diagramitem.cpp \
    ../OSLD/gate.cpp \
    ../OSLD/osldgraphicsengine.cpp \
    ../OSLD/rootitempathscene.cpp \
    ../OSLD/subdiagram.cpp

HEADERS  += mainwindow.h \
    ../OSLD/block.h \
    ../OSLD/connector.h \
    ../OSLD/descriptionfilereader.h \
    ../OSLD/diagramitem.h \
    ../OSLD/gate.h \
    ../OSLD/osldgraphicsengine.h \
    ../OSLD/rootitempathscene.h \
    ../OSLD/subdiagram.h

FORMS    += mainwindow.ui

DISTFILES += \
    stimulator.sqlite3
