#ifndef ENTRY_H
#define ENTRY_H

#include <QString>

#define UL unsigned long int
#define LD long double

class Entry
{
public:
    // variables
    UL CAJA;
    UL CANTIDAD;
    UL CANT_POR_CAJA;
    QString CLAVE;
    QString Description;
    LD PRECIO;
    LD IMPORTE;
    LD btm_left_num;

    //functions
    Entry(UL CAJA = 0, UL CANTIDAD = 0, UL CANT_POR_CAJA = 0,
          QString CLAVE = "", QString Description = "",
          LD PRECIO = 0., LD IMPORTE = 0., LD btm_left_num = 0.);

};

#endif // ENTRY_H
