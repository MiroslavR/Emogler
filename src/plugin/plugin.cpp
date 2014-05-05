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

#include "plugin.h"

#include <QApplication>
#include <QDebug>

Plugin::Plugin(Category cat, QString id, QPluginLoader * loader) :
    mCategory(cat),
    mID(id),
    mLoader(loader),
    mInterface(nullptr),
    mLoaded(false)
{
    Q_ASSERT(cat != Plugin::Unknown);
}

Plugin::~Plugin()
{
    unload();
}

Plugin::ErrorCode Plugin::load()
{
    Q_ASSERT(mLoader != nullptr);

    QObject * root = mLoader->instance();

    if (!root) {
        qDebug() << QString("Failed to load %1: %2").arg(mID).arg(mLoader->errorString());
        mLoader->unload();
        return LoaderError;
    }

    Q_ASSERT(mInterface == nullptr);

    BasePluginInterface * iface = qobject_cast<BasePluginInterface *>(root);
    if (!iface) {
        return BadInterface;
    }

    if (mCategory == Extension && !qobject_cast<ExtensionInterface *>(root)) {
        return BadInterface;
    }

    if (mCategory == Protocol && !qobject_cast<ProtocolInterface *>(root)) {
        return BadInterface;
    }

    mInterface = iface;
    mIcon = mInterface->icon();

    mLoaded = true;
    return Success;
}

void Plugin::unload()
{
    Q_ASSERT(mLoader != nullptr);

    if (mLoaded) {
        mLoader->unload();
        mInterface = nullptr;
        mLoaded = false;
    }
}

void Plugin::toggleLoad()
{
    if (mLoaded) {
        unload();
    } else {
        load();
    }
}

Plugin::Category Plugin::category() const
{
    return mCategory;
}

QString Plugin::id() const
{
    return mID;
}

QString Plugin::name(const QLocale & loc) const
{
    if (!mName.contains(loc)) {
        QLocale def("en_US");
        if (mName.contains(def)) {
            return mName[def];
        }
        return QString();
    }
    return mName[loc];
}

QIcon Plugin::icon() const
{
    return mIcon;
}

QString Plugin::author() const
{
    return mAuthor;
}

QString Plugin::version() const
{
    return mVersion;
}

QString Plugin::description(const QLocale & loc) const
{
    if (!mDescription.contains(loc)) {
        QLocale def("en_US");
        if (mDescription.contains(def)) {
            return mDescription[def];
        }
        return QString();
    }
    return mDescription[loc];
}

QPluginLoader * Plugin::loader() const
{
    return mLoader;
}

BasePluginInterface *Plugin::interface() const
{
    return mInterface;
}

bool Plugin::isLoaded() const
{
    return mLoaded;
}

bool Plugin::hasAuthor()
{
    return (!mAuthor.isEmpty());
}

bool Plugin::hasVersion()
{
    return (!mVersion.isEmpty());
}

bool Plugin::hasDescription(const QLocale & loc)
{
    return (mDescription.contains(loc) || mDescription.contains(QLocale("en_US")));
}

bool Plugin::hasDependencies()
{
    return mDependencies.size();
}

const QStringList & Plugin::dependencies() const
{
    return mDependencies;
}

void Plugin::setName(const QLocale & loc, const QString & name)
{
    mName[loc] = name;
}

void Plugin::setIcon(const QIcon & icon)
{
    mIcon = icon;
}

void Plugin::setAuthor(const QString & author)
{
    mAuthor = author;
}

void Plugin::setVersion(const QString & ver)
{
    mVersion = ver;
}

void Plugin::setDescription(const QLocale & loc, const QString & desc)
{
    mDescription[loc] = desc;
}

void Plugin::setInterface(BasePluginInterface * iface)
{
    mInterface = iface;
}

void Plugin::addDependency(const QString & dep)
{
    mDependencies << dep;
}

QString Plugin::categoryString(Plugin::Category cat)
{
    switch (cat) {
        case Plugin::Extension:
            return QApplication::translate("Plugin", "Extension");
        case Plugin::Protocol:
            return QApplication::translate("Plugin", "Protocol");
        default:
            return QApplication::translate("Plugin", "Unknown");
    }
}

