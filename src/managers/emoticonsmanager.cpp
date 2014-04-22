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
#include <algorithm>

#include "managers/conversationmanager.h"

EmoticonsManager::EmoticonsManager(QSettings & s)
    : mSettings(s)
{
    /*loadPack("standard");
    loadPack("pixelized");*/

    QDir currentDir = QDir::currentPath();
    currentDir.cd("data/emoticons");
    loadAllPacks(currentDir);
}

EmoticonsManager::LoadState EmoticonsManager::loadPack(const QString & packName)
{
    QUrl schemaUrl("qrc:///data/emoticons/pack.xsd");

    QXmlSchema schema;
    schema.load(schemaUrl);

    if (!schema.isValid()) {
        return InvalidSchema;
    }

    QFile file("./data/emoticons/" + packName + "/pack.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not read emoticon pack" << packName;
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

    EmoticonPack * pack = new EmoticonPack(packName);

    if (mSettings.childGroups().contains("Packs")) {
        mSettings.beginGroup("Packs");
        if (mSettings.childGroups().contains(packName)) {
            mSettings.beginGroup(packName);
            if (mSettings.contains("enabled"))
                pack->setEnabled(mSettings.value("enabled", false).toBool());

            int priority = mSettings.value("priority", -1).toInt();
            pack->setPriority(priority);

            mSettings.endGroup();
        }
        mSettings.endGroup();
    }

    if (root.tagName() == "pack") {
        QDomElement name = root.firstChildElement("name");
        pack->setName(name.text());

        QDomElement entries = root.firstChildElement("emoticons");
        for (QDomNode sm = entries.firstChild(); !sm.isNull(); sm = sm.nextSibling()) {
            (*pack) << Emoticon(sm.toElement().text(), sm.toElement().attribute("path", ""));
        }
    }

    pack->sort();
    mPacks << pack;

    file.close();
    return Success;
}

void EmoticonsManager::loadAllPacks(const QDir & d)
{
    QDir dir = d;
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable);

    QStringList entries = dir.entryList();

    if (entries.contains("standard")) {
        // make sure the 'standard' pack is placed first
        int standard_index = entries.indexOf("standard");
        if (standard_index != 0) {
            entries.swap(0, standard_index);
        }
    }

    for (const QString & entry : entries) {
        loadPack(entry);
    }
    sortPacksByPriority();
}

void EmoticonsManager::saveSettings(QSettings &s)
{
    for (int i = 0; i < mPacks.size(); i++) {
        s.beginGroup("Packs/" + mPacks.at(i)->id());
        s.setValue("enabled", mPacks.at(i)->isEnabled());
        s.setValue("priority", mPacks.at(i)->priority()/*mPacks.size() - i*/);
        s.endGroup();
    }
}

void EmoticonsManager::swapPacks(int i, int j)
{
    mPacks.swap(i, j);
}

void EmoticonsManager::sortPacksByPriority()
{
    std::sort(mPacks.begin(), mPacks.end(), [](EmoticonPack * a, EmoticonPack * b) -> bool {
        return a->priority() > b->priority();
    });
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
