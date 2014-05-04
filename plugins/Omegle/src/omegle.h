#ifndef OMEGLE_H
#define OMEGLE_H

#include "omegle_global.h"
#include <protocolinterface.h>
#include <QObject>

class OMEGLESHARED_EXPORT Omegle : public QObject, ProtocolInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "emogler.protocol" FILE "plugininfo.json")
    Q_INTERFACES(ProtocolInterface)

    public:
        Omegle();
};

#endif // OMEGLE_H
