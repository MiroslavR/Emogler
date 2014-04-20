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

#ifndef EMOTICONPACK_H
#define EMOTICONPACK_H

#include "emoticon.h"

class EmoticonPack
{
    public:
        EmoticonPack(const QString & packName);
        ~EmoticonPack();

        void setEnabled(bool enabled);
        void setName(const QString & name);
        void setAuthor(const QString & author);

        static bool sortFunc(const Emoticon &, const Emoticon &);
        void sort();

        void append(const Emoticon & emot);

        void operator<<(const Emoticon & emot);
        Emoticon & operator[](int);
        const QList<Emoticon> & list() const;
        const QList<Emoticon> & sortedList() const;
        QString id() const;
        bool isEnabled() const;
        QString name() const;
        QString author() const;

    private:
        QString mId;
        bool mEnabled;
        QString mName;
        QString mAuthor;
        QList<Emoticon> mList;
        QList<Emoticon> mListSorted;
};

#endif // EMOTICONPACK_H
