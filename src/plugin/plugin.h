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

#ifndef PLUGIN_H
#define PLUGIN_H

#include <QString>
#include <QStringList>
#include <QLocale>
#include <QPluginLoader>
#include <QIcon>

#include "interfaces/baseplugininterface.h"
#include "interfaces/extensioninterface.h"
#include "interfaces/protocolinterface.h"

class Plugin
{
    public:
        enum ErrorCode {
            Success = 1,
            UnknownFailure = 0,
            LoaderError = -1,
            BadInterface = -2
        };

        enum Category {
            Unknown,
            Extension,
            Protocol
        };

        Plugin(Category cat, QString id, QPluginLoader * loader);
        virtual ~Plugin();

        virtual ErrorCode load();
        virtual void unload();
        void toggleLoad();

        Category category() const;
        QString id() const;
        QString name(const QLocale & loc) const;
        QIcon icon() const;
        QString author() const;
        QString version() const;
        QString description(const QLocale & loc) const;
        const QStringList & dependencies() const;
        QPluginLoader * loader() const;
        BasePluginInterface * interface() const;
        bool isLoaded() const;

        bool hasAuthor();
        bool hasVersion();
        bool hasDescription(const QLocale & loc);
        bool hasDependencies();

        void setName(const QLocale & loc, const QString & name);
        void setIcon(const QIcon & icon);
        void setAuthor(const QString & author);
        void setVersion(const QString & ver);
        void setDescription(const QLocale & loc, const QString & desc);
        void setInterface(BasePluginInterface * iface);
        void addDependency(const QString & dep);

        static QString categoryString(Category cat);

    private:
        Category mCategory;
        QString mID;
        QHash<QLocale, QString> mName;
        QIcon mIcon;
        QString mAuthor;
        QString mVersion;
        QHash<QLocale, QString> mDescription;
        QStringList mDependencies;
        QPluginLoader * mLoader;
        BasePluginInterface * mInterface;
        bool mLoaded;
};

inline uint qHash(const QLocale & key, uint /*seed = 0*/)
{
    return qHash(key.name());
}

Q_DECLARE_METATYPE(Plugin *)


#endif // PLUGIN_H
