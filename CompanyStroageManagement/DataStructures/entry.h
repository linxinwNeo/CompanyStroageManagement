#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include "DataStructures/Model.h"
#include <QVector>

#define EntryPtr QSharedPointer<Entry>

class Entry
{
public:
    // variables
    QString CLAVE; // 货号
    QString ContainerID; // 集装箱号
    double CAJA; // 箱数
    unsigned long CANTIDAD; // 总个数
    unsigned long CANT_POR_CAJA; // 每箱个数
    QString Description_SPAN; // 品名（西语）
    QString Description_CN; // 品名（中文）
    double PRECIO; // 单价
    double IMPORTE; // 总价

    //functions
    Entry(double CAJA = 0., unsigned long CANTIDAD = 0., unsigned long CANT_POR_CAJA = 0.,
          QString CLAVE = "", QString ContainerID = "",
          QString Description_SPAN = "", QString Description_CN = "",
          double PRECIO = 0., double IMPORTE = 0.);

    ModelPtr get_corresponding_model() const;
    QVector<QString> view_values() const;
};

#endif // ENTRY_H
