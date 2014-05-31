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

#include "conversation.h"

Conversation::Conversation()
{
}

void Conversation::addPlainTextMessage(Conversation::Message type, const QDateTime & dt, const QString & msg, const QString & who)
{
    emit plainTextMessage(type, dt, msg, who);
}

void Conversation::addCustomMessage(const QDateTime & dt, const QString & msg)
{
    emit customMessage(dt, msg);
}
