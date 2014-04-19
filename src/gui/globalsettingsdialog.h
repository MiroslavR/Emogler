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

#ifndef GLOBALSETTINGSDIALOG_H
#define GLOBALSETTINGSDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "managers/emoglercore.h"

namespace Ui {
class GlobalSettingsDialog;
}

class GlobalSettingsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit GlobalSettingsDialog(QWidget * parent = 0);
        ~GlobalSettingsDialog();

    private slots:
        /*void mapSetting(QWidget * w, const QString & set, const QString & prop = QString::null);
        void commit();*/
        void saveSettings();
        void loadSettings();
        void fieldChanged();

        void populatePluginTree();

        void on_pluginTree_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);

        void on_detailsButton_clicked();

protected:
            void changeEvent(QEvent * e);

    private:
        struct MappedSetting {
            QString set;
            bool user;
        };

        void mapSetting(QWidget * w, const QString & set, bool user = true);

        Ui::GlobalSettingsDialog * ui;
        EmoglerCore & core;
        QHash<QWidget *, MappedSetting> mWidgetMap;
        //QHash<QWidget *, QString> mWidgetMap;
};

#endif // GLOBALSETTINGSDIALOG_H
