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

#include "emoglerwindow.h"
#include "ui_emoglerwindow.h"

#include "conversationwidget.h"
#include "globalsettingsdialog.h"
#include "managers/emoglercore.h"

EmoglerWindow::EmoglerWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::EmoglerWindow)
{
    ui->setupUi(this);

    QWidget * conWidget = new ConversationWidget(this);
    setCentralWidget(conWidget);

    QDir dir = QDir::current();
    dir.cd("plugins/");
    EmoglerCore::instance().pluginManager().loadAllLibraries(dir);
}

EmoglerWindow::~EmoglerWindow()
{
    delete ui;
}

void EmoglerWindow::changeEvent(QEvent * e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void EmoglerWindow::on_actionSettings_triggered()
{
    ui->actionSettings->setEnabled(false);

    GlobalSettingsDialog * dlg = new GlobalSettingsDialog(this);
    dlg->setModal(false);
    connect(dlg, &QDialog::finished, this, [this](int) {
        ui->actionSettings->setEnabled(true);
    });
    dlg->show();
}
