#include "DataStructures/entry.h"

Entry::Entry(double CAJA, UL CANTIDAD, UL CANT_POR_CAJA ,
             QString CLAVE, QString Description, double PRECIO,
             double IMPORTE, double btm_left_num)
{
    this->CAJA = CAJA;
    this->CANTIDAD = CANTIDAD;
    this->CANT_POR_CAJA = CANT_POR_CAJA;
    this->CLAVE = CLAVE;
    this->Description = Description;
    this->PRECIO = PRECIO;
    this->IMPORTE = IMPORTE;
    this->btm_left_num = btm_left_num;
}
