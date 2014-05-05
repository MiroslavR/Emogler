QT       += gui network

TARGET = Omegle
TEMPLATE = lib

QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_HIDESYMS

DEFINES += OMEGLE_LIBRARY

INCLUDEPATH += src/
INCLUDEPATH += ../../src/interfaces

CONFIG += c++11 plugin no_plugin_name_prefix

SOURCES += src/omegle.cpp

HEADERS += src/omegle.h\
        src/omegle_global.h

unix {
    target.path = ../
    INSTALLS += target
}

OTHER_FILES += \
    src/plugininfo.json

RESOURCES += \
    resources.qrc
