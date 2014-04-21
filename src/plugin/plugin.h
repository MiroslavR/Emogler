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

class Plugin
{
    public:
        enum Category {
            Unknown,
            Extension,
            Protocol
        };

        Plugin(Category cat, QString id, QPluginLoader * loader);

        Category category() const;
        QString id() const;
        QString name(const QLocale & loc) const;
        QString author() const;
        QString version() const;
        QString description(const QLocale & loc) const;
        const QStringList & dependencies() const;
        QPluginLoader * loader() const;
        bool isLoaded() const;

        bool hasAuthor();
        bool hasVersion();
        bool hasDescription(const QLocale & loc);
        bool hasDependencies();

        void setName(const QLocale & loc, const QString & name);
        void setAuthor(const QString & author);
        void setVersion(const QString & ver);
        void setDescription(const QLocale & loc, const QString & desc);

        static QString categoryString(Category cat);

    private:
        Category mCategory;
        QString mID;
        QHash<QLocale, QString> mName;
        QString mAuthor;
        QString mVersion;
        QHash<QLocale, QString> mDescription;
        QStringList mDependencies;
        QPluginLoader * mLoader;
        bool mLoaded;
};

inline uint qHash(const QLocale & key, uint /*seed = 0*/)
{
    return qHash(key.name());
}

Q_DECLARE_METATYPE(Plugin *)


#endif // PLUGIN_H
