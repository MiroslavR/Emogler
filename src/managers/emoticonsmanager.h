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

#ifndef EMOTICONSMANAGER_H
#define EMOTICONSMANAGER_H

#include <QString>

#include "emoticons/emoticon.h"
#include "emoticons/emoticonpack.h"

class EmoticonsManager
{
    public:
        enum LoadState {
            Success = 1,
            UnknownFailure = 0,
            InvalidSchema = -1,
            SetOpenError = -2,
            InvalidSet = -3
        };

        EmoticonsManager();

        LoadState loadPack(const QString & pack);

        void swapPacks(int i, int j);
        const QList<EmoticonPack *> & packs() const;

        QString emotize(const QString & text);

    private:
        QList<EmoticonPack *> mPacks;
};

#endif // EMOTICONSMANAGER_H
