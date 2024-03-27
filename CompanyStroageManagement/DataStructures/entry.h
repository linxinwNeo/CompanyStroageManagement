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
    QString MODEL_CODE; // 货号 / CLAVE
    QString ContainerID; // 集装箱号
    unsigned long NUM_PIECES; // 个数 / CANTIDAD
    unsigned long NUM_PIECES_PER_BOX; // 每箱个数 / CANT_POR_CAJA
    QString Description_SPAN; // 品名（西语）
    QString Description_CN; // 品名（中文）
    double PRICE_PER_PIECE; // 单价 / PRECIO
    double TOTAL; // 总价 / TOTAL

    //functions
    Entry(unsigned long NUM_PIECES = 0., unsigned long NUM_PIECES_PER_BOX = 0.,
          QString MODEL_CODE = "", QString ContainerID = "",
          QString Description_SPAN = "", QString Description_CN = "",
          double PRIZE_PER_PIECE = 0.);

    QSharedPointer<Model> get_corresponding_model() const;
    QVector<QString> view_values() const;

    double num_boxes() const;
};

#endif // ENTRY_H
