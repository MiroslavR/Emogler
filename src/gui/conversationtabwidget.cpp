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

#include "conversationtabwidget.h"
#include "ui_conversationtabwidget.h"

#include <QTime>

ConversationTabWidget::ConversationTabWidget(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::ConversationTabWidget),
    mConversation(nullptr)
{
    ui->setupUi(this);
    //ui->textBrowser->addMessage("Hello, world!", QTime::currentTime());
}

void ConversationTabWidget::changeEvent(QEvent * e)
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

ConversationTabWidget::~ConversationTabWidget()
{
    delete ui;
}

void ConversationTabWidget::setConversation(Conversation * convo)
{
    if (mConversation == convo)
        return;

    if (mConversation)
        removeConversation();

    mConversation = convo;
    connect(mConversation, &Conversation::plainTextMessage, ui->textBrowser, [this](Conversation::Message type, const QDateTime & dt, const QString & msg, const QString & who) {
        ui->textBrowser->addMessage(type, dt.time(), msg, who);
    });
}

void ConversationTabWidget::removeConversation()
{
    if (mConversation == nullptr)
        return;

    mConversation->disconnect(ui->textBrowser);
}

const Conversation * ConversationTabWidget::conversation() const
{
    return mConversation;
}
