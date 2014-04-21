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

#include "emoticonsmanager.h"

#include <QDomDocument>
#include <QFile>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include "managers/conversationmanager.h"

EmoticonsManager::EmoticonsManager()
{
    loadPack("standard");
    loadPack("pixelized");
}

EmoticonsManager::LoadState EmoticonsManager::loadPack(const QString & pack)
{
    QUrl schemaUrl("qrc:///data/emoticons/pack.xsd");

    QXmlSchema schema;
    schema.load(schemaUrl);

    if (!schema.isValid()) {
        return InvalidSchema;
    }

    QFile file("./data/emoticons/" + pack + "/pack.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not read emoticon pack" << pack;
        return SetOpenError;
    }

    QXmlSchemaValidator validator(schema);
    if (!validator.validate(&file, QUrl::fromLocalFile(file.fileName()))) {
        file.close();
        return InvalidSet;
    }

    file.seek(0);

    QDomDocument doc;
    doc.setContent(&file);
    QDomElement root = doc.documentElement();

    EmoticonPack * set = new EmoticonPack(pack);
    if (root.tagName() == "pack") {
        QDomElement name = root.firstChildElement("name");
        set->setName(name.text());

        QDomElement entries = root.firstChildElement("emoticons");
        for (QDomNode sm = entries.firstChild(); !sm.isNull(); sm = sm.nextSibling()) {
            (*set) << Emoticon(sm.toElement().text(), sm.toElement().attribute("path", ""));
        }
    }

    set->sort();
    mPacks << set;

    file.close();
    return Success;
}

void EmoticonsManager::swapPacks(int i, int j)
{
    mPacks.swap(i, j);
}

const QList<EmoticonPack *> & EmoticonsManager::packs() const
{
    return mPacks;
}

QString EmoticonsManager::emotize(const QString & text)
{
    QString ret = "";
    int last = 0;
    for (int i = 0; i < text.size(); i++) {
        for (EmoticonPack * pack : mPacks) {
            for (const Emoticon & emot : pack->sortedList()) {
                if (text.mid(i).length() >= emot.face().length()) {
                    if (text.mid(i, emot.face().length()) == emot.face()) {
                        ret += ConversationManager::optimize(text.mid(last, i - last));
                        ret += QString("<img src=\"data/emoticons/%1/%2\" title=\"%3\">").arg(pack->id().toHtmlEscaped()).arg(emot.icon().toHtmlEscaped()).arg(emot.face().toHtmlEscaped());
                        i += emot.face().length() - 1;
                        last = i + 1;
                    }
                }
            }
        }
    }
    ret += ConversationManager::optimize(text.mid(last));
    return ret;
}
