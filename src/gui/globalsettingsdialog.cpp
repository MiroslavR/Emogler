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
    BaseWidgetSettings(EmoglerCore::instance().settings(), this),
    ui(new Ui::GlobalSettingsDialog),
    core(EmoglerCore::instance()),
    mEmoticonsModel(new EmoticonsTableModel(core.emoticonsManager(), this))
{
    ui->setupUi(this);

    ui->listWidget->setFixedWidth(ui->listWidget->sizeHintForColumn(0) + 20);

    ui->emoticonsView->setModel(mEmoticonsModel);
    connect(mEmoticonsModel, &EmoticonsTableModel::checkStateChanged, this, &GlobalSettingsDialog::fieldChanged);

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
    mapSetting(ui->emoticonsEnabledCheckBox, "Conversation/enableEmoticons", true);
    loadSettings();

    connect(this, &QDialog::accepted, this, &GlobalSettingsDialog::saveSettings);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &GlobalSettingsDialog::saveSettings);

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
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
    BaseWidgetSettings::saveSettings();

    core.settings().setValue(mWidgetMap[ui->languageComboBox].set, ui->languageComboBox->currentData().toString());
    core.setLanguage(ui->languageComboBox->currentData().toString());

    mEmoticonsModel->commit();

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

    core.emoticonsManager().saveSettings(core.settings());

    /*core.settings().beginWriteArray("emoticons/pack");
    QList<EmoticonPack *> packs = core.emoticonsManager().packs();
    for (int i = 0; i < packs.size(); i++) {
        core.settings().setArrayIndex(i);
        core.settings().setValue("enabled", packs.at(i)->isEnabled());
    }
    core.settings().endArray();*/
}

void GlobalSettingsDialog::loadSettings()
{
    BaseWidgetSettings::loadSettings();

    for (int i = 0; i < ui->languageComboBox->count(); i++) {
        auto setLang = QLocale(core.language()); /*QLocale(core.settings().value(mWidgetMap[ui->languageComboBox].set).toString()).language();*/
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

void GlobalSettingsDialog::moveEmoticonPack(int step)
{
    // there must be a row that is selected
    if (!ui->emoticonsView->selectionModel()->selectedRows().size())
        return;

    // move selected row by step
    QModelIndex index = ui->emoticonsView->selectionModel()->selectedRows()[0];
    if (mEmoticonsModel->switchRows(index.row(), index.row() + step)) {
        ui->emoticonsView->selectionModel()->select(index, QItemSelectionModel::Deselect | QItemSelectionModel::Rows);
        ui->emoticonsView->selectionModel()->select(mEmoticonsModel->index(index.row() + step, index.column()), QItemSelectionModel::Select | QItemSelectionModel::Rows);

        fieldChanged();
    }
}

void GlobalSettingsDialog::on_upButton_clicked()
{
    moveEmoticonPack(-1);
}

void GlobalSettingsDialog::on_downButton_clicked()
{
    moveEmoticonPack(1);
}
