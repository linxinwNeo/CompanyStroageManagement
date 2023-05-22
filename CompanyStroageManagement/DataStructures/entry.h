#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include "Predefined.h"

class Entry
{
public:
    // variables
    double CAJA;
    UL CANTIDAD;
    UL CANT_POR_CAJA;
    QString CLAVE;
    QString Description;
    double PRECIO;
    double IMPORTE;
    double btm_left_num;

    //functions
    Entry(double CAJA = 0, UL CANTIDAD = 0, UL CANT_POR_CAJA = 0,
          QString CLAVE = "", QString Description = "",
          double PRECIO = 0., double IMPORTE = 0., double btm_left_num = 0.);

};

#endif // ENTRY_H
