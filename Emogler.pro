#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T14:10:32
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Emogler
TEMPLATE = app

CONFIG += c++11
QMAKE_CXXFLAGS += -Wall

INCLUDEPATH += src/

TRANSLATIONS_DIR = $$[QT_INSTALL_TRANSLATIONS]
DEFINES += "TRANSLATIONS_DIR=\\\"$${TRANSLATIONS_DIR}\\\""

include(src/src.pri)

RESOURCES += \
    icons.qrc \
    langs.qrc \
    xml.qrc

TRANSLATIONS = data/langs/emo_en.ts \
    data/langs/emo_sk.ts
