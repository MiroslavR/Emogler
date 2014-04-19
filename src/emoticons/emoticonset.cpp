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

#include "emoticonset.h"

EmoticonSet::EmoticonSet(const QString & packName) :
    mPackName(packName)
{
}

void EmoticonSet::setName(const QString & name)
{
    mName = name;
}

void EmoticonSet::setAuthor(const QString & author)
{
    mAuthor = author;
}

const QList<Emoticon *> & EmoticonSet::list() const
{
    return mList;
}

const QList<Emoticon *> & EmoticonSet::sortedList() const
{
    return mListSorted;
}

bool EmoticonSet::sortFunc(Emoticon * em1, Emoticon * em2)
{
    return em1->face().length() > em2->face().length();
}

void EmoticonSet::sort()
{
    mListSorted = mList;
    qSort(mListSorted.begin(), mListSorted.end(), sortFunc);
}

Emoticon * EmoticonSet::operator[](int i)
{
    return mList[i];
}

QString EmoticonSet::name() const
{
    return mName;
}

QString EmoticonSet::packName() const
{
    return mPackName;
}

QString EmoticonSet::author() const
{
    return mAuthor;
}

EmoticonSet::~EmoticonSet()
{
    while (!mList.isEmpty()) {
        delete mList.takeFirst();
    }
}
