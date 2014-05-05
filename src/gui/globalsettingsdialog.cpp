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

    ui->languageComboBox->addItem(tr("System language"));

    QDir dir(":/data/langs");
    QStringList fileNames = dir.entryList(QStringList("emo_*.qm"));
    for (QString locale : fileNames) {
        locale.truncate(locale.lastIndexOf("."));
        locale.remove(0, 4);

        ui->languageComboBox->addItem(QIcon(QString(":/data/flags/%1.png").arg(locale)), QLocale::languageToString(QLocale(locale).language()), QLocale(locale).name());
    }

    ui->languageComboBox->setCurrentIndex(0);

    mapSetting(ui->showSystrayCheckBox, "showSystray", true);
    mapSetting(ui->languageComboBox, "language", false);
    mapSetting(ui->emoticonsEnabledCheckBox, "Conversation/enableEmoticons", true);
    loadSettings();

    connect(ui->languageComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &GlobalSettingsDialog::fieldChanged);
    connect(this, &QDialog::accepted, this, &GlobalSettingsDialog::saveSettings);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &GlobalSettingsDialog::saveSettings);

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void GlobalSettingsDialog::fieldChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void GlobalSettingsDialog::changeEvent(QEvent * e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            ui->languageComboBox->setItemText(0, tr("System language"));
            break;
        default:
            break;
    }
}

void GlobalSettingsDialog::saveSettings()
{
    BaseWidgetSettings::saveSettings();

    mEmoticonsModel->commit();

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

    core.emoticonsManager().saveSettings(core.settings());

    if (ui->languageComboBox->currentIndex() == 0) { // system language
        core.settings().setValue(mWidgetMap[ui->languageComboBox].set, "system");
        core.setSystemLanguage(true);
    } else {
        core.settings().setValue(mWidgetMap[ui->languageComboBox].set, ui->languageComboBox->currentData().toString());
        core.setLanguage(ui->languageComboBox->currentData().toString());
        core.setSystemLanguage(false);
    }

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

    if (core.isSystemLanguage()) {
        ui->languageComboBox->setCurrentIndex(0);
        return;
    }

    for (int i = 1; i < ui->languageComboBox->count(); i++) {
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
