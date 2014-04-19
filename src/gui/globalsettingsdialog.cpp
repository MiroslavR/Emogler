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

#include "globalsettingsdialog.h"
#include "ui_globalsettingsdialog.h"

#include <QPushButton>
#include <QMetaProperty>
#include <QLocale>
#include <QDebug>

#include "managers/emoglercore.h"
#include "gui/plugindetailsdialog.h"

GlobalSettingsDialog::GlobalSettingsDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::GlobalSettingsDialog),
    core(EmoglerCore::instance())
{
    ui->setupUi(this);

    ui->listWidget->setFixedWidth(ui->listWidget->sizeHintForColumn(0) + 20);

    QDir dir(":/data/langs");
    QStringList fileNames = dir.entryList(QStringList("emo_*.qm"));
    for (QString locale : fileNames) {
        locale.truncate(locale.lastIndexOf("."));
        locale.remove(0, 4);

        ui->languageComboBox->addItem(QIcon(QString(":/data/flags/%1.png").arg(locale)), QLocale::languageToString(QLocale(locale).language()), QLocale(locale).name());
    }

    ui->languageComboBox->setCurrentIndex(0);

    populatePluginTree();

    mapSetting(ui->showSystrayCheckBox, "showSystray", true);
    mapSetting(ui->languageComboBox, "language", false);
    loadSettings();

    connect(this, &QDialog::accepted, this, &GlobalSettingsDialog::saveSettings);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &GlobalSettingsDialog::saveSettings);

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void GlobalSettingsDialog::mapSetting(QWidget * w, const QString & set, bool user)
{
    mWidgetMap[w] = {set, user};
    w->setProperty(w->metaObject()->userProperty().name(), core.settings().value(mWidgetMap[w].set));
    connect(w, w->metaObject()->userProperty().notifySignal(), this, metaObject()->method(metaObject()->indexOfSlot("fieldChanged()")));
}

void GlobalSettingsDialog::fieldChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void GlobalSettingsDialog::populatePluginTree()
{
    // remove children of top level items
    for (int i = 0; i < ui->pluginTree->topLevelItemCount(); i++) {
        QTreeWidgetItem * it = ui->pluginTree->topLevelItem(i);
        while (it->childCount())
            it->takeChild(0);
    }

    EmoglerCore & core = EmoglerCore::instance();
    PluginManager & pman = core.pluginManager();
    for (Plugin * pl : pman.plugins()) {
        QTreeWidgetItem * it;

        switch (pl->category()) {
            case Plugin::Extension:
                it = new QTreeWidgetItem(ui->pluginTree->topLevelItem(0));
                break;
            case Plugin::Protocol:
                it = new QTreeWidgetItem(ui->pluginTree->topLevelItem(1));
                break;
            default:
                continue;
        }

        it->setText(0, pl->name(core.language()));
        it->setText(1, pl->description(core.language()));
        it->setCheckState(0, (pl->isLoaded()) ? Qt::Checked : Qt::Unchecked);
        it->setData(0, Qt::UserRole, QVariant::fromValue<Plugin *>(pl));
    }

    ui->pluginTree->expandAll();
    ui->pluginTree->resizeColumnToContents(0);
}

void GlobalSettingsDialog::changeEvent(QEvent * e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            populatePluginTree();
            break;
        default:
            break;
    }
}

void GlobalSettingsDialog::saveSettings()
{
    foreach (QWidget * w, mWidgetMap.keys()) {
        if (mWidgetMap[w].user)
            core.settings().setValue(mWidgetMap[w].set, w->property(w->metaObject()->userProperty().name()));
    }

    core.settings().setValue(mWidgetMap[ui->languageComboBox].set, ui->languageComboBox->currentData().toString());
    core.setLanguage(ui->languageComboBox->currentData().toString());
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void GlobalSettingsDialog::loadSettings()
{
    foreach (QWidget * w, mWidgetMap.keys()) {
        if (mWidgetMap[w].user)
            w->setProperty(w->metaObject()->userProperty().name(), core.settings().value(mWidgetMap[w].set));
    }

    for (int i = 0; i < ui->languageComboBox->count(); i++) {
        auto setLang = QLocale(core.settings().value(mWidgetMap[ui->languageComboBox].set).toString()).language();
        auto dataLang = QLocale(ui->languageComboBox->itemData(i).toString()).language();

        if (setLang == dataLang) {
            ui->languageComboBox->setCurrentIndex(i);
            break;
        }
    }
}

GlobalSettingsDialog::~GlobalSettingsDialog()
{
    delete ui;
}

void GlobalSettingsDialog::on_pluginTree_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * /*previous*/)
{
    QVariant data = current->data(0, Qt::UserRole);
    ui->detailsButton->setEnabled(data.isValid());
}

void GlobalSettingsDialog::on_detailsButton_clicked()
{
    QVariant data = ui->pluginTree->currentItem()->data(0, Qt::UserRole);
    if (data.isValid()) {
        Plugin * pl = data.value<Plugin *>();
        PluginDetailsDialog dlg(this, *pl);
        dlg.exec();
    }
}