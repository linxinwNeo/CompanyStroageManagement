#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <QString>
#include <QDebug>

class Client_info
{
public:
    // variables
    QString CLIENTE;
    QString DOMICILIO;
    QString CIUDAD;
    QString RFC;
    QString AGENTE;
    QString CONDICIONES;
    QString bottom_left_num;
    double DISCOUNT;

    // functions
    Client_info(QString CLIENTE = "", QString DOMICILIO = "", QString CIUDAD = "",
                QString RFC = "", QString AGENTE = "",
                QString CONDICIONES = "", QString bottom_left_num = "", double DISCOUNT = 0.);
};

#endif // CLIENT_INFO_H
