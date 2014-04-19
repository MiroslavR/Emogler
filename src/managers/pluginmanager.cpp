/*************************************************************************
  Emogler
  Copyright (C) 2011-2014 Miroslav Remák

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

#include "pluginmanager.h"

#include <QPluginLoader>
#include <QLibrary>
#include <QLocale>

#include <QDebug>

PluginManager::PluginManager()
{
}

int PluginManager::loadLibrary(const QString & name)
{
    QPluginLoader * loader = new QPluginLoader(QString("./plugins/%1").arg(name));

    //qDebug() << loader->metaData() << loader->metaData()["MetaData"].contains("name");

    QObject * root = loader->instance();

    qDebug() << "loading" << name;
    if (!root) {
        qDebug() << QString("Failed to load %1: %2").arg(name).arg(loader->errorString());
        loader->unload();
        return LoaderError;
    }

    /*
     * Check meta data:
     * category, name - required
     * author, version, description, dependencies - optional
    */

        //||  (!loader->metaData().contains("name") || loader->metaData().value("name").toString().isEmpty())

    qDebug() << loader->metaData().value("name").toString();

    loader->unload();
    return Unknown;
}

PluginManager::ErrorCode PluginManager::addLibrary(const QString & id)
{
    QPluginLoader * loader = new QPluginLoader(QString("./plugins/%1").arg(id));

    if (!loader->metaData().contains("MetaData")) {
        delete loader;
        return BadMetaData;
    }

    QJsonObject info = loader->metaData()["MetaData"].toObject();
    qDebug() << info;

    if (!info.contains("category") || !info.contains("name")) {
        delete loader;
        return BadMetaData;
    }

    QString cat = info["category"].toString();

    if (!isValidCategory(cat)) {
        delete loader;
        return BadMetaData;
    }

    Plugin * pl = new Plugin(strToCategory(cat), id, loader);

    if (info["name"].isObject()) {
        QJsonObject langs = info["name"].toObject();
        for (QString lang : langs.keys()) {
             pl->setName(QLocale(lang), langs[lang].toString());
        }
    } else {
        pl->setName(QLocale("en_US"), info["name"].toString());
    }

    if (info.contains("author"))
        pl->setAuthor(info["author"].toString());

    if (info.contains("description")) {
        if (info["description"].isObject()) {
            QJsonObject langs = info["description"].toObject();
            for (QString lang : langs.keys()) {
                pl->setDescription(QLocale(lang), langs[lang].toString());
            }
        } else {
            pl->setDescription(QLocale("en_US"), info["description"].toString());
        }
    }

    if (info.contains("version"))
        pl->setVersion(info["version"].toString());

    qDebug() << pl->name(QLocale("sk_SK")) << pl->description(QLocale("en_US"));

    mPlugins[id] = pl;

    return Success;
}

void PluginManager::loadAllLibraries(const QDir & path)
{
    QDir dir = path;
    dir.setFilter(QDir::Files | QDir::Readable);

    QFileInfoList entries = path.entryInfoList();
    for (QFileInfo info : entries) {
        if (!QLibrary::isLibrary(info.fileName()))
            continue;

        //loadLibrary(info.fileName());
        addLibrary(info.fileName());
    }
}

const QHash<QString, Plugin *> & PluginManager::plugins() const
{
    return mPlugins;
}

Plugin::Category PluginManager::strToCategory(const QString & str) const
{
    if (str == CAT_EXTENSION_NAME) {
        return Plugin::Extension;
    } else if (str == CAT_PROTOCOL_NAME) {
        return Plugin::Protocol;
    }

    return Plugin::Unknown;
}

bool PluginManager::isValidCategory(const QString & str) const
{
    return ((str == CAT_EXTENSION_NAME) || (str == CAT_PROTOCOL_NAME));
}
