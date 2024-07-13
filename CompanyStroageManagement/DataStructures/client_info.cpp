#include "DataStructures/client_info.h"


Client::Client(QString ID, QString CLIENTE,
               QString DOMICILIO, QString CIUDAD,
               QString RFC, QString AGENTE,
               QString CONDICIONES)
{
    this->m_ID = ID;
    this->m_clientName = CLIENTE;
    this->m_DOMICILIO = DOMICILIO;
    this->m_CIUDAD = CIUDAD;
    this->m_RFC = RFC;
    this->m_AGENTE = AGENTE;
    this->m_CONDICIONES = CONDICIONES;
}

void Client::searchResult(QVector<QString> &items) const
{
    items.clear();
    items.reserve(7);
    items.push_back(this->m_ID);
    items.push_back(this->m_clientName);
    items.push_back(m_DOMICILIO);
    items.push_back(this->m_CIUDAD);
    items.push_back(m_RFC);
    items.push_back(m_AGENTE);
    items.push_back(m_CONDICIONES);
}


Client_info::Client_info(QString ID, QString CLIENTE,
                         QString DOMICILIO, QString CIUDAD,
                         QString RFC, QString AGENTE,
                         QString CONDICIONES, double TOTAL_NUM_BOXES, double DISCOUNT)
    : Client(ID, CLIENTE, DOMICILIO, CIUDAD, RFC, AGENTE, CONDICIONES)
{
    this->m_DISCOUNT = DISCOUNT;
    this->m_TOTAL_NUM_BOXES = TOTAL_NUM_BOXES;
}
