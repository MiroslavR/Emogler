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

#ifndef CONVERSATIONWIDGET_H
#define CONVERSATIONWIDGET_H

#include <QWidget>

namespace Ui {
class ConversationWidget;
}

class ConversationWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ConversationWidget(QWidget * parent = 0);
        ~ConversationWidget();

    private slots:
        void on_emoticonsButton_clicked();

    private:
            Ui::ConversationWidget * ui;
};

#endif // CONVERSATIONWIDGET_H
