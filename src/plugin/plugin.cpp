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

Plugin::Plugin(Category cat, QString id, QPluginLoader * loader) :
    mCategory(cat),
    mID(id),
    mLoader(loader),
    mLoaded(false)
{
    Q_ASSERT(cat != Plugin::Unknown);
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
        return QString();
    }
    return mName[loc];
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
        return QString();
    }
    return mDescription[loc];
}

QPluginLoader * const Plugin::loader() const
{
    return mLoader;
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
    return (mDescription.contains(loc));
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

