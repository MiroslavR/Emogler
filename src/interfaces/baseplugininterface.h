#ifndef BASEPLUGININTERFACE_H
#define BASEPLUGININTERFACE_H

#include <QtPlugin>
#include <QIcon>

class BasePluginInterface
{
    public:
        virtual ~BasePluginInterface() {}
        virtual QIcon icon() = 0;
};

Q_DECLARE_INTERFACE(BasePluginInterface, "emogler.plugin")

#endif // BASEPLUGININTERFACE_H
