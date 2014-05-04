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

#ifndef EMOGLERCORE_H
#define EMOGLERCORE_H

#include <QObject>
#include <QSettings>
#include <QTranslator>

#include "managers/pluginmanager.h"
#include "managers/conversationmanager.h"
#include "managers/emoticonsmanager.h"

#include "conversation/conversation.h"

class EmoglerCore : public QObject
{
    Q_OBJECT

    public:
        explicit EmoglerCore(QObject * parent = 0);

        static EmoglerCore & instance();

        QSettings & settings();

        QString language() const;
        void setLanguage(const QString & lng);

        PluginManager & pluginManager();
        ConversationManager & conversationManager();
        EmoticonsManager & emoticonsManager();

signals:
        void languageChanged(const QString & lng);

    public slots:

    private:
        static EmoglerCore * mInstance;

        QSettings mSettings;
        QTranslator mTranslator;
        QTranslator mTranslatorQt;
        QString mLanguage;

        PluginManager mPluginManager;
        ConversationManager mConversationManager;
        EmoticonsManager mEmoticonsManager;
};

#endif // EMOGLERCORE_H
