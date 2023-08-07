#include "DataStructures/entry.h"

Entry::Entry(double CAJA, UL CANTIDAD, UL CANT_POR_CAJA ,
             QString CLAVE, QString Description,
             double PRECIO, double IMPORTE)
{
    this->CAJA = CAJA;
    this->CANTIDAD = CANTIDAD;
    this->CANT_POR_CAJA = CANT_POR_CAJA;
    this->CLAVE = CLAVE;
    this->Description = Description;
    this->PRECIO = PRECIO;
    this->IMPORTE = IMPORTE;
}
