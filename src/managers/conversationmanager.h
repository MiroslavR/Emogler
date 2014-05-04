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

#ifndef CONVERSATIONMANAGER_H
#define CONVERSATIONMANAGER_H

#include <QObject>
#include <QSettings>

#include "conversation/conversation.h"

class ConversationManager : public QObject
{
    Q_OBJECT

    public:
        explicit ConversationManager(QSettings & s, QObject * parent = 0);

        const QList<Conversation *> conversations() const;
        Conversation * newConversation(const QString & prId);

        static QString optimize(const QString & text);

    signals:

    public slots:

    private:
        QSettings & mSettings;
        QList<Conversation *> mConversations;
};

#endif // CONVERSATIONMANAGER_H
