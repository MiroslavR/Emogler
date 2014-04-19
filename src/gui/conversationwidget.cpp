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

#include "conversationtabwidget.h"

ConversationWidget::ConversationWidget(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::ConversationWidget)
{
    ui->setupUi(this);

    QWidget * wTab = new ConversationTabWidget(this);
    ui->conversationTabWidget->addTab(wTab, "Test");

    QWidget * wTab2 = new ConversationTabWidget(this);
    ui->conversationTabWidget->addTab(wTab2, "Test2");
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}
