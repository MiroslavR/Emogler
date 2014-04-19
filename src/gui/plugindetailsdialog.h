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

#ifndef PLUGINDETAILSDIALOG_H
#define PLUGINDETAILSDIALOG_H

#include <QDialog>

#include "plugin/plugin.h"

namespace Ui {
class PluginDetailsDialog;
}

class PluginDetailsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit PluginDetailsDialog(QWidget * parent, Plugin & pl);
        ~PluginDetailsDialog();

    protected:
        void changeEvent(QEvent * e);

    private slots:
        void populate();

    private:
        Ui::PluginDetailsDialog * ui;
        Plugin & mPlugin;
};

#endif // PLUGINDETAILSDIALOG_H
