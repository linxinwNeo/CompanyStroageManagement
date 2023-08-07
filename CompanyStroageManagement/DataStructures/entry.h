#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include "Predefined.h"

#define EntryPtr QSharedPointer<Entry>

class Entry
{
public:
    // variables
    double CAJA; // 箱数
    UL CANTIDAD; // 总件数
    UL CANT_POR_CAJA; // 每箱件数
    QString CLAVE; // 货号
    QString Description; // 品名（西语）
    double PRECIO; // 单价
    double IMPORTE; // 总价

    //functions
    Entry(double CAJA = 0., UL CANTIDAD = 0., UL CANT_POR_CAJA = 0.,
          QString CLAVE = "", QString Description = "",
          double PRECIO = 0., double IMPORTE = 0.);
};

#endif // ENTRY_H
