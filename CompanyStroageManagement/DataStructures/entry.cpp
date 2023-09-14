#include "DataStructures/entry.h"
#include "GlobalVars.h"

Entry::Entry(double CAJA, UL CANTIDAD, UL CANT_POR_CAJA ,
             QString CLAVE, QString ContainerID,
             QString Description_SPAN, QString Description_CN,
             double PRECIO, double IMPORTE)
{
    this->CLAVE = CLAVE;
    this->CAJA = CAJA;
    this->CANTIDAD = CANTIDAD;
    this->CANT_POR_CAJA = CANT_POR_CAJA;
    this->Description_SPAN = Description_SPAN;
    this->Description_CN = Description_CN;
    this->PRECIO = PRECIO;
    this->IMPORTE = IMPORTE;
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
        QString::number(CAJA, 'f', 2),
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
