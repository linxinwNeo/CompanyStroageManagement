#ifndef PREDEFINED_H
#define PREDEFINED_H

#define UL unsigned long int
#define UI unsigned int
#define LD long double

#include <QtGlobal>
#ifdef Q_OS_WIN
#define WIN
#endif

#ifdef Q_OS_MACX
#define MAC
#endif

#endif // PREDEFINED_H
