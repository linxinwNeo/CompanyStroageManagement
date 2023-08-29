#include "DataStructures/entry.h"
#include "Others/handle_containerID.h"

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

    // we do special treatment to the ContainerID
    this->ContainerID = ContainerID;
    handle_ContainerID(this->ContainerID);
}
