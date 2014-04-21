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
    w->setProperty(w->metaObject()->userProperty().name(), mSettings.value(mWidgetMap[w].set));
    QObject::connect(w, w->metaObject()->userProperty().notifySignal(), mParent, mParent->metaObject()->method(mParent->metaObject()->indexOfSlot("fieldChanged()")));
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
