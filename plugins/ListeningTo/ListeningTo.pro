#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T16:57:22
#
#-------------------------------------------------

QT       -= gui

TARGET = ListeningTo
TEMPLATE = lib

DEFINES += LISTENINGTO_LIBRARY

INCLUDEPATH += src/
INCLUDEPATH += ../../src/interfaces

CONFIG += plugin no_plugin_name_prefix

SOURCES += src/listeningto.cpp

HEADERS += src/listeningto.h\
        src/listeningto_global.h

unix {
    target.path = ../
    INSTALLS += target
}

OTHER_FILES += \
    src/plugininfo.json
