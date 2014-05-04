#ifndef OMEGLE_GLOBAL_H
#define OMEGLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OMEGLE_LIBRARY)
#  define OMEGLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define OMEGLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OMEGLE_GLOBAL_H
