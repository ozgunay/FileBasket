#ifndef FBLIB_GLOBAL_H
#define FBLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FBLIB_LIBRARY)
#  define FBLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FBLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FBLIB_GLOBAL_H
