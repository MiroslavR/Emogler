#ifndef BASEWIDGETSETTINGS_H
#define BASEWIDGETSETTINGS_H

#include <QWidget>
#include <QSettings>

class BaseWidgetSettings
{
    public:
        BaseWidgetSettings(QSettings & s, QObject * parent);

    protected:
        struct MappedSetting {
            QString set;
            bool user;
        };

        void mapSetting(QWidget * w, const QString & set, bool user = true);
        virtual void saveSettings();
        virtual void loadSettings();
        virtual void fieldChanged() = 0;

        QHash<QWidget *, MappedSetting> mWidgetMap;

    private:
        QObject * mParent;
        QSettings & mSettings;
};

#endif // BASEWIDGETSETTINGS_H
