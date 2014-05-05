/*************************************************************************
  Emogler
  Copyright (C) 2014 Miroslav Remák

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************/

#include "emoglercore.h"

#include <QApplication>
#include <QLocale>
#include <QLibraryInfo>
#include <QDebug>

EmoglerCore * EmoglerCore::mInstance = nullptr;

EmoglerCore::EmoglerCore(QObject * parent) :
    QObject(parent),
    mSettings(QSettings::IniFormat, QSettings::UserScope, "Emogler", "settings"),
    mSystemLanguage(true),
    mConversationManager(mSettings),
    mEmoticonsManager(mSettings)
{
    mLanguage = mSettings.value("language", "system").toString();
    bool sys = mLanguage == "system";
    setSystemLanguage(sys);

    if (!sys) {
        setLanguage(mLanguage);
    }

    //qDebug() << "hmm" << QLocale("zh").name() << QLocale("zh_TW").name() << QLocale("zh_CN").name();
}

EmoglerCore & EmoglerCore::instance()
{
    if (mInstance == nullptr) {
        mInstance = new EmoglerCore;
    }

    return *mInstance;
}

QSettings & EmoglerCore::settings()
{
    return mSettings;
}

QString EmoglerCore::language() const
{
    return mLanguage;
}

bool EmoglerCore::isSystemLanguage()
{
    return mSystemLanguage;
}

void EmoglerCore::setLanguage(const QString & lng)
{
    qApp->removeTranslator(&mTranslator);
    qApp->removeTranslator(&mTranslatorQt);

    mTranslator.load("emo_" + lng, ":/data/langs");
    mTranslatorQt.load("qt_" + lng, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&mTranslator);
    qApp->installTranslator(&mTranslatorQt);

    mLanguage = lng;
    emit languageChanged(lng);
}

void EmoglerCore::setSystemLanguage(bool sys)
{
    if (sys) {
        setLanguage(QLocale::system().name());
    }
    mSystemLanguage = sys;
}

PluginManager & EmoglerCore::pluginManager()
{
    return mPluginManager;
}

ConversationManager & EmoglerCore::conversationManager()
{
    return mConversationManager;
}

EmoticonsManager & EmoglerCore::emoticonsManager()
{
    return mEmoticonsManager;
}
