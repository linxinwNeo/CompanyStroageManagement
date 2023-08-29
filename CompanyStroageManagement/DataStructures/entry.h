#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include "Predefined.h"

#define EntryPtr QSharedPointer<Entry>

class Entry
{
public:
    // variables
    QString CLAVE; // 货号
    QString ContainerID; // 集装箱号
    double CAJA; // 箱数
    UL CANTIDAD; // 总件数
    UL CANT_POR_CAJA; // 每箱件数
    QString Description_SPAN; // 品名（西语）
    QString Description_CN; // 品名（中文）
    double PRECIO; // 单价
    double IMPORTE; // 总价

    //functions
    Entry(double CAJA = 0., UL CANTIDAD = 0., UL CANT_POR_CAJA = 0.,
          QString CLAVE = "", QString ContainerID = "",
          QString Description_SPAN = "", QString Description_CN = "",
          double PRECIO = 0., double IMPORTE = 0.);
};

#endif // ENTRY_H
