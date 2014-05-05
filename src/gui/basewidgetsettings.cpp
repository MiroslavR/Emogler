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

#include "basewidgetsettings.h"

#include <QMetaProperty>

BaseWidgetSettings::BaseWidgetSettings(QSettings & s, QObject * parent)
    : mSettings(s),
      mParent(parent)
{
}

void BaseWidgetSettings::mapSetting(QWidget * w, const QString & set, bool user)
{
    mWidgetMap[w] = {set, user};
    if (user) {
        w->setProperty(w->metaObject()->userProperty().name(), mSettings.value(mWidgetMap[w].set));
        QObject::connect(w, w->metaObject()->userProperty().notifySignal(), mParent, mParent->metaObject()->method(mParent->metaObject()->indexOfSlot("fieldChanged()")));
    }
}


void BaseWidgetSettings::saveSettings()
{
    foreach (QWidget * w, mWidgetMap.keys()) {
        if (mWidgetMap[w].user)
            mSettings.setValue(mWidgetMap[w].set, w->property(w->metaObject()->userProperty().name()));
    }
}

void BaseWidgetSettings::loadSettings()
{
    foreach (QWidget * w, mWidgetMap.keys()) {
        if (mWidgetMap[w].user)
            w->setProperty(w->metaObject()->userProperty().name(), mSettings.value(mWidgetMap[w].set));
    }
}
