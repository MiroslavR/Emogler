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

#ifndef EMOTICONSET_H
#define EMOTICONSET_H

#include "emoticon.h"

class EmoticonSet
{
    private:
        QString mName;
        QString mAuthor;
        QString mPackName;
        QList<Emoticon *> mList;
        QList<Emoticon *> mListSorted;

    public:
        EmoticonSet(const QString & packName);
        ~EmoticonSet();

        void setName(const QString & name);
        void setAuthor(const QString & author);

        static bool sortFunc(Emoticon *, Emoticon *);
        void sort();

        Emoticon * operator[](int);
        const QList<Emoticon *> & list() const;
        const QList<Emoticon *> & sortedList() const;
        QString name() const;
        QString packName() const;
        QString author() const;
};

#endif // EMOTICONSET_H
