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

#ifndef EMOGLERWINDOW_H
#define EMOGLERWINDOW_H

#include <QMainWindow>

namespace Ui {
class EmoglerWindow;
}

class EmoglerWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit EmoglerWindow(QWidget * parent = 0);
        ~EmoglerWindow();

    protected:
        void changeEvent(QEvent * e);

    private slots:
        void on_actionSettings_triggered();

        void on_actionPlugins_triggered();

private:
            Ui::EmoglerWindow * ui;
};

#endif // EMOGLERWINDOW_H
