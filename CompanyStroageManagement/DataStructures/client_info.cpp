#include "DataStructures/client_info.h"

Client_info::Client_info(QString CLIENTE, QString DOMICILIO, QString CIUDAD,
                         QString RFC, QString AGENTE,
                         QString CONDICIONES, double TOTAL_NUM_BOXES, double DISCOUNT)
{
    this->CLIENTE = CLIENTE;
    this->DOMICILIO = DOMICILIO;
    this->CIUDAD = CIUDAD;
    this->RFC = RFC;
    this->AGENTE = AGENTE;
    this->CONDICIONES = CONDICIONES;
    this->DISCOUNT = DISCOUNT;
    this->TOTAL_NUM_BOXES = TOTAL_NUM_BOXES;
}
