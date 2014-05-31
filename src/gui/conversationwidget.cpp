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
    ui(new Ui::ConversationWidget),
    mCurrentProtocol(nullptr)
{
    ui->setupUi(this);

    PlusButton * plusButton = new PlusButton(ui->conversationTabWidget);
    plusButton->setMinimumHeight(32);
    ui->conversationTabWidget->setCornerWidget(plusButton);

    connect(plusButton, &PlusButton::currentProtocolChanged, this, [this](Plugin * pl) {
        mCurrentProtocol = pl;
    });

    connect(plusButton, &QPushButton::clicked, this, &ConversationWidget::addTabCurrentProtocol);
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}

void ConversationWidget::changeEvent(QEvent * e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void ConversationWidget::addTab(const Plugin * pl)
{
    ConversationManager & cman = EmoglerCore::instance().conversationManager();
    Conversation * convo = cman.newConversation(pl->id());
    if (convo) {
        ConversationTabWidget * wTab = new ConversationTabWidget(this);
        wTab->setConversation(convo);
        ui->conversationTabWidget->addTab(wTab, pl->icon(), "Test");
    }
}

void ConversationWidget::addTabCurrentProtocol()
{
    if (mCurrentProtocol == nullptr)
        return;

    addTab(mCurrentProtocol);
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
