#include "client_info.h"

Client_info::Client_info(QString CLIENTE, QString DOMICILIO, QString CIUDAD,
                         QString RFC, QString AGENTE,
                         QString CONDICIONES, QString bottom_left_num, double DISCOUNT)
{
    this->CLIENTE = CLIENTE;
    this->DOMICILIO = DOMICILIO;
    this->CIUDAD = CIUDAD;
    this->RFC = RFC;
    this->AGENTE = AGENTE;
    this->CONDICIONES = CONDICIONES;
    this->DISCOUNT = DISCOUNT;
    this->bottom_left_num = bottom_left_num;
}
