#include "omegle.h"

#include <QDebug>

Omegle::Omegle()
{
    qDebug() << "Omegle constructor";
}

QIcon Omegle::icon()
{
    return QIcon(":/plugins/Omegle/icon.png");
}
