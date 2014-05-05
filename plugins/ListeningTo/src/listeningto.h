#ifndef LISTENINGTO_H
#define LISTENINGTO_H

#include "listeningto_global.h"
#include <extensioninterface.h>
#include <QObject>

class LISTENINGTOSHARED_EXPORT ListeningTo : public QObject, ExtensionInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "emogler.extension" FILE "plugininfo.json")
    Q_INTERFACES(BasePluginInterface ExtensionInterface)

    public:
        ListeningTo();

        virtual QIcon icon() override;
};

#endif // LISTENINGTO_H
