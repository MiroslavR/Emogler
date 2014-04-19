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

#include "plugindetailsdialog.h"
#include "ui_plugindetailsdialog.h"

#include "managers/emoglercore.h"

PluginDetailsDialog::PluginDetailsDialog(QWidget * parent, Plugin & pl) :
    QDialog(parent),
    ui(new Ui::PluginDetailsDialog),
    mPlugin(pl)
{
    ui->setupUi(this);

    populate();
}

PluginDetailsDialog::~PluginDetailsDialog()
{
    delete ui;
}

void PluginDetailsDialog::changeEvent(QEvent * e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            populate();
            break;
        default:
            break;
    }
}

void PluginDetailsDialog::populate()
{
    ui->dependenciesListWidget->clear();

    ui->categoryLabel_2->setText(Plugin::categoryString(mPlugin.category()));
    ui->nameLabel_2->setText(mPlugin.name(EmoglerCore::instance().language()));

    if (mPlugin.hasAuthor())
        ui->authorLabel_2->setText(mPlugin.author());

    if (mPlugin.hasDescription(EmoglerCore::instance().language()))
        ui->descriptionTextBrowser->setText(mPlugin.description(EmoglerCore::instance().language()));

    if (mPlugin.hasVersion())
        ui->versionLabel_2->setText(mPlugin.version());
}
