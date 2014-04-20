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

#include "emoticonpack.h"

EmoticonPack::EmoticonPack(const QString & id) :
    mId(id),
    mEnabled(false)
{
}

void EmoticonPack::setName(const QString & name)
{
    mName = name;
}

void EmoticonPack::setAuthor(const QString & author)
{
    mAuthor = author;
}

const QList<Emoticon> & EmoticonPack::list() const
{
    return mList;
}

const QList<Emoticon> & EmoticonPack::sortedList() const
{
    return mListSorted;
}

bool EmoticonPack::sortFunc(const Emoticon & em1, const Emoticon & em2)
{
    return em1.face().length() > em2.face().length();
}

void EmoticonPack::sort()
{
    mListSorted = mList;
    qSort(mListSorted.begin(), mListSorted.end(), sortFunc);
}

void EmoticonPack::append(const Emoticon & emot)
{
    mList << emot;
}

void EmoticonPack::operator<<(const Emoticon & emot)
{
    append(emot);
}

Emoticon & EmoticonPack::operator[](int i)
{
    return mList[i];
}

QString EmoticonPack::id() const
{
    return mId;
}

bool EmoticonPack::isEnabled() const
{
    return mEnabled;
}


QString EmoticonPack::name() const
{
    return mName;
}

QString EmoticonPack::author() const
{
    return mAuthor;
}

EmoticonPack::~EmoticonPack()
{
    mList.clear();
}

void EmoticonPack::setEnabled(bool enabled)
{
    mEnabled = enabled;
}
