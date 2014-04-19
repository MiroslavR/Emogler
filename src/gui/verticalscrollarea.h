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

#ifndef VERTICALSCROLLAREA_H
#define VERTICALSCROLLAREA_H

#include <QScrollArea>
#include <QEvent>

class VerticalScrollArea : public QScrollArea
{
    Q_OBJECT

    public:
        explicit VerticalScrollArea(QWidget * parent = 0);

    protected:
        virtual bool eventFilter(QObject * obj, QEvent * e);

    signals:

    public slots:

};

#endif // VERTICALSCROLLAREA_H
