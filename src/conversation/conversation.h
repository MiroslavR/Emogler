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

#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QObject>
#include <QDateTime>

class Conversation : public QObject
{
    Q_OBJECT

    public:
        enum class Message {
            Regular = 0x1,
            You = 0x2,
            Stranger = 0x4,
            Information = 0x8,
            Error = 0x10,
            AddLine = 0x20
        };

        Conversation();

        void addPlainTextMessage(Message type, const QDateTime & dt, const QString & msg, const QString & who = "");
        void addCustomMessage(const QDateTime & dt, const QString & msg);

    signals:
        void plainTextMessage(Message type, const QDateTime & dt, const QString & msg, const QString & who);
        void customMessage(const QDateTime & dt, const QString & msg);
};

#endif // CONVERSATION_H
