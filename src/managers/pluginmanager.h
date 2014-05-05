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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDir>
#include <QMap>
#include <QString>

#include "interfaces/extensioninterface.h"
#include "plugin/plugin.h"

class PluginManager
{
    public:
        enum ErrorCode {
            Success = 1,
            Unknown = 0,
            LoaderError = -1,
            BadMetaData = -2,
            BadInterface = -3
        };

        PluginManager();

        ErrorCode addLibrary(const QString & id);
        void loadAllLibraries(const QDir & path);

        const QHash<QString, Plugin *> & plugins() const;

        Plugin::Category strToCategory(const QString & str) const;
        bool isValidCategory(const QString & str) const;

    private:
        QHash<QString, Plugin *> mPlugins;

        constexpr static auto CAT_EXTENSION_NAME = "extension";
        constexpr static auto CAT_PROTOCOL_NAME = "protocol";
};

#endif // PLUGINMANAGER_H
