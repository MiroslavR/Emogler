/*************************************************************************
  Emogler
  Copyright (C) 2011-2014 MiroslavR

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

#ifndef EMOTICONSMENU_H
#define EMOTICONSMENU_H

#include <QFrame>
#include <QAction>
#include <QEvent>

class EmoticonsMenu : public QFrame
{
    Q_OBJECT

    public:
        EmoticonsMenu(QWidget * parent = 0);
        EmoticonsMenu(int w, int h, QWidget * parent = 0);

        void init();

        QAction * addItem(const QIcon & icon);
        void addItems(const QList<QAction *> & items);
        QRect fieldFromAction(QAction * ac);
        QAction * itemAt(const QPoint & glPos);
        QAction * itemAt(int x, int y);
        int indexOf(QAction * ac);

        QSize fieldSize();
        QSize calcSize();

        void setFieldSize(const QSize & fieldSize);
        void setFieldSize(int w, int h);

        void updateSize();

        void setCloseOnTrigger(bool close);
        bool closeOnTrigger();

    signals:
        void triggered(QAction *);

    protected:
        virtual bool event(QEvent * e) override;
        virtual void resizeEvent(QResizeEvent *) override;
        virtual void paintEvent(QPaintEvent *) override;
        virtual void mouseReleaseEvent(QMouseEvent *) override;
        virtual void mouseMoveEvent(QMouseEvent *) override;
        virtual void keyPressEvent(QKeyEvent *) override;


    private:
        QList<QAction *> mActions;
        QPoint mMousePos;

        int mSelectedIndex;
        QSize mFieldSize;
        bool mCloseOnTrigger;

};

#endif // EMOTICONSMENU_H
