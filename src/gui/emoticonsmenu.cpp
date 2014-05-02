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

#include "emoticonsmenu.h"

#include <QHelpEvent>
#include <QStyleOptionMenuItem>
#include <QToolTip>
#include <QPainter>
#include <QtMath>
#include <QtDebug>

EmoticonsMenu::EmoticonsMenu(QWidget * parent) : QFrame(parent, Qt::Popup | Qt::Window), mSelectedIndex(-1), mFieldSize(32, 32), mCloseOnTrigger(true)
{
    init();
}

EmoticonsMenu::EmoticonsMenu(int w, int h, QWidget * parent) : QFrame(parent, Qt::Popup | Qt::Window), mSelectedIndex(-1), mFieldSize(QSize(w, h)), mCloseOnTrigger(true)
{
    init();
}

void EmoticonsMenu::init()
{
    setFrameStyle(QFrame::Panel);
    setLineWidth(1);

    setAttribute(Qt::WA_DeleteOnClose, true);
    resize(200, 200);
    setMouseTracking(true);
}

QSize EmoticonsMenu::fieldSize()
{
    return mFieldSize;
}

QSize EmoticonsMenu::calcSize()
{
    int w = qCeil(qSqrt(mActions.size()));
    int h = qCeil(floor(mActions.size()) / w);
    return QSize(w, h);
}

void EmoticonsMenu::setFieldSize(const QSize & size)
{
    mFieldSize = size;
    update();
}

void EmoticonsMenu::setFieldSize(int w, int h)
{
    setFieldSize(QSize(w, h));
}

void EmoticonsMenu::updateSize()
{
    QSize sz = calcSize();
    if ((width() != sz.width() * mFieldSize.width()) && (height() != sz.height() * mFieldSize.height()))
        resize(sz.width() * mFieldSize.width(), sz.height() * mFieldSize.height());
}

QAction * EmoticonsMenu::itemAt(const QPoint & glPos)
{
    QPoint po = mapFromGlobal(glPos);

    if (!rect().contains(po)) {
        return nullptr;
    }

    int dx = po.x() / mFieldSize.width();
    int dy = po.y() / mFieldSize.height();
    int w = width() / mFieldSize.width();

    int i = (dy * w + dx);
    if (i < 0 || i >= mActions.size())
        return nullptr;

    return mActions[i];
}

int EmoticonsMenu::indexOf(QAction * ac)
{
    return mActions.indexOf(ac);
}

QRect EmoticonsMenu::fieldFromAction(QAction * ac)
{
    int i = mActions.indexOf(ac);
    int w = width() / mFieldSize.width();
    return QRect((i % w) * mFieldSize.width(), (i / w) * mFieldSize.height(), mFieldSize.width(), mFieldSize.height());
}

QAction * EmoticonsMenu::itemAt(int x, int y)
{
    return itemAt(QPoint(x, y));
}

QAction * EmoticonsMenu::addItem(const QIcon & icon)
{
    QAction * act = new QAction(this);
    act->setIcon(icon);

    mActions << act;
    update();
    updateSize();
    return act;
}

void EmoticonsMenu::addItems(const QList<QAction *> & items)
{
    for (QAction * act : items) {
        mActions << act;
    }
    update();
    updateSize();
}

bool EmoticonsMenu::event(QEvent * e)
{
    if (e->type() == QEvent::ToolTip) {
        QHelpEvent * helpEvent = static_cast<QHelpEvent *> (e);
        if (helpEvent->type() == QEvent::ToolTip) {
            QAction * item = itemAt(helpEvent->globalPos());
            if (item) {
                if (item->toolTip().size()) {
                    QToolTip::showText(helpEvent->globalPos(), item->toolTip(), this, fieldFromAction(item));
                }
            } else {
                QToolTip::hideText();
                e->ignore();
            }
            return true;
        } else {
            QToolTip::hideText();
        }
    }
    return QFrame::event(e);
}

void EmoticonsMenu::resizeEvent(QResizeEvent * e)
{
    move(parentWidget()->mapToGlobal(QPoint(0, -e->size().height())));
    QFrame::resizeEvent(e);
}

void EmoticonsMenu::paintEvent(QPaintEvent * e)
{
    QFrame::paintEvent(e);
    QPainter paint(this);

    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(this);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
    menuOpt.tabWidth = 0;
    style()->drawPrimitive(QStyle::PE_PanelMenu, &menuOpt, &paint, this);

    QSize sz = calcSize();

    for (int i = 0; i < mActions.size(); i++) {
        int rows = i / sz.width();
        int cols = i % sz.width();

        QRect field(cols * mFieldSize.width(), rows * mFieldSize.height(), mFieldSize.width(), mFieldSize.height());

        if (i == mSelectedIndex) {
            QStyleOptionMenuItem opt;
            opt.initFrom(this);
            opt.palette = palette();
            opt.state = QStyle::State_Active | QStyle::State_Enabled | QStyle::State_Selected;
            opt.font = mActions[i]->font().resolve(font());
            opt.fontMetrics = QFontMetrics(opt.font);
            opt.text = "";
            opt.menuItemType = QStyleOptionMenuItem::DefaultItem;
            opt.menuRect = rect();
            opt.rect = field;

            style()->drawControl(QStyle::CE_MenuItem, &opt, &paint);
        }

        QIcon icon = mActions[i]->icon();
        QSize iconSize = icon.actualSize(QSize(mFieldSize.width(), mFieldSize.height()));

        QPoint c = field.center();
        field.setSize(iconSize);
        field.moveCenter(c);
        paint.drawPixmap(field, icon.pixmap(iconSize.width(), iconSize.height()));
    }
}

void EmoticonsMenu::mouseMoveEvent(QMouseEvent * e)
{
    if (rect().contains(e->x(), e->y())) {
        QAction * item = itemAt(e->globalX(), e->globalY());
        if (item) {
            item->hover();
            mSelectedIndex = indexOf(item);
        } else {
            mSelectedIndex = -1;
        }
    } else {
        mSelectedIndex = -1;
    }
    update();

    QFrame::mouseMoveEvent(e);
}

void EmoticonsMenu::mouseReleaseEvent(QMouseEvent * e)
{
    if (mSelectedIndex != -1) {
        QAction * ac = mActions[mSelectedIndex];
        ac->trigger();
        emit triggered(ac);

        if (mCloseOnTrigger)
            close();
    }

    QFrame::mouseReleaseEvent(e);
}

void EmoticonsMenu::keyPressEvent(QKeyEvent * e)
{
    switch (e->key()) {
        case Qt::UpArrow:
        case Qt::DownArrow:
        case Qt::LeftArrow:
        case Qt::RightArrow:
        default:
            break;
    }

    QFrame::keyPressEvent(e);
}

bool EmoticonsMenu::closeOnTrigger()
{
    return mCloseOnTrigger;
}

void EmoticonsMenu::setCloseOnTrigger(bool close)
{
    mCloseOnTrigger = close;
}
