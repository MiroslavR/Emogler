QT       += gui

TARGET = ListeningTo
TEMPLATE = lib

QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_HIDESYMS

DEFINES += LISTENINGTO_LIBRARY

INCLUDEPATH += src/
INCLUDEPATH += ../../src/interfaces

CONFIG += c++11 plugin no_plugin_name_prefix

SOURCES += src/listeningto.cpp

HEADERS += src/listeningto.h\
        src/listeningto_global.h

unix {
    target.path = ../
    INSTALLS += target
}

OTHER_FILES += \
    src/plugininfo.json
