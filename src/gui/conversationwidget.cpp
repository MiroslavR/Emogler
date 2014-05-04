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

#include "conversationwidget.h"
#include "ui_conversationwidget.h"

#include "plusbutton.h"
#include "conversationtabwidget.h"
#include "emoticonsmenu.h"
#include "managers/emoglercore.h"

ConversationWidget::ConversationWidget(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::ConversationWidget)
{
    ui->setupUi(this);

    ui->conversationTabWidget->setCornerWidget(new PlusButton(this));

    QWidget * wTab = new ConversationTabWidget(this);
    ui->conversationTabWidget->addTab(wTab, "Test");

    QWidget * wTab2 = new ConversationTabWidget(this);
    ui->conversationTabWidget->addTab(wTab2, "Test2");
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}

void ConversationWidget::addTab(const QString & prId)
{

}

void ConversationWidget::on_emoticonsButton_clicked()
{
    EmoticonsMenu * emenu = new EmoticonsMenu(ui->emoticonsButton);
    EmoticonsManager & eman = EmoglerCore::instance().emoticonsManager();
    const QSettings & s = EmoglerCore::instance().settings();
    QList<QAction *> emoActions;
    for (const EmoticonPack * pack : eman.packs()) {
        if (!pack->isEnabled())
            continue;

        for (const Emoticon emot : pack->list()) {
            QAction * item = new QAction(emenu);
            item->setIcon(QIcon(QString("data/emoticons/%1/%2").arg(pack->id()).arg(emot.icon())));
            item->setToolTip(emot.face());
            emoActions << item;
        }
    }
    emenu->addItems(emoActions);
    emenu->show();
}
