QT       += network

QT       -= gui

TARGET = Omegle
TEMPLATE = lib

DEFINES += OMEGLE_LIBRARY

INCLUDEPATH += src/
INCLUDEPATH += ../../src/interfaces

CONFIG += plugin no_plugin_name_prefix

SOURCES += src/omegle.cpp

HEADERS += src/omegle.h\
        src/omegle_global.h

unix {
    target.path = ../
    INSTALLS += target
}

OTHER_FILES += \
    src/plugininfo.json
