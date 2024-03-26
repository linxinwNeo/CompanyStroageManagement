#include "DataStructures/entry.h"
#include "GlobalVars.h"

Entry::Entry(unsigned long NUM_PIECES, unsigned long NUM_PIECES_PER_BOX,
             QString MODEL_CODE, QString ContainerID,
             QString Description_SPAN, QString Description_CN,
             double PRIZE_PER_PIECE)
{
    this->CLAVE = MODEL_CODE;
    this->CANTIDAD = NUM_PIECES;
    this->CANT_POR_CAJA = NUM_PIECES_PER_BOX;
    this->Description_SPAN = Description_SPAN;
    this->Description_CN = Description_CN;
    this->PRECIO = PRIZE_PER_PIECE;
    this->IMPORTE = ((double)NUM_PIECES) * PRIZE_PER_PIECE;
    this->ContainerID = ContainerID;
}


// search inventory, and return the model with matching info
ModelPtr Entry::get_corresponding_model() const
{
    const QString& MODELCODE = this->CLAVE;
    const QString& CONTAINER_ID = this->ContainerID;
    ModelPtr model = inventory.get_Model(MODELCODE, CONTAINER_ID);

    return model;
}


QVector<QString> Entry::view_values() const
{
    return {
        QString::number(this->num_boxes(), 'f', 2),
        QString::number(CANTIDAD),
        QString::number(CANT_POR_CAJA),
        CLAVE,
        Description_SPAN,
        Description_CN,
        QString::number(PRECIO, 'f', 2),
        QString::number(IMPORTE, 'f', 2),
        ContainerID
    };
}


// 计算箱数
double Entry::num_boxes() const
{
    const double number_of_pieces = this->CANTIDAD;
    const double number_of_pieces_per_box = this->CANT_POR_CAJA;
    const double num = number_of_pieces / number_of_pieces_per_box;
    return num;
}
