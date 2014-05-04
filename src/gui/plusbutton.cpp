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

#include "plusbutton.h"

#include "managers/emoglercore.h"
#include <QActionGroup>
#include <QMenu>
#include <QDebug>

PlusButton::PlusButton(QWidget * parent) :
    QToolButton(parent)
{
    setIcon(QIcon(":/data/gui/add.png"));
}

void PlusButton::contextMenuEvent(QContextMenuEvent * e)
{
    EmoglerCore & core = EmoglerCore::instance();
    PluginManager & pman = core.pluginManager();

    QMenu menu;
    QActionGroup * group = new QActionGroup(&menu);
    for (const QString & id : pman.plugins().keys()) {
        Plugin * pl = pman.plugins().value(id);
        qDebug() << "PlusButton" << pl->name(core.language()) << pl->isLoaded();
        if (pl->category() == Plugin::Protocol && pl->isLoaded()) {
            QAction * ac = new QAction(QIcon(), pl->name(core.language()), group);
            menu.addAction(ac);
        }
    }

    connect(group, &QActionGroup::triggered, this, [](QAction * ac) {
        // change protocol
    });
    menu.exec(e->globalPos());
}
