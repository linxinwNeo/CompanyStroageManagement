#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <QString>
#include <QDebug>

class Client{
public:
    QString m_ID; //客户号码
    QString m_clientName; // 客户名字
    QString m_DOMICILIO; // 地址
    QString m_CIUDAD; // 城市
    QString m_RFC;
    QString m_AGENTE; //代理
    QString m_CONDICIONES; //付款方式

    Client(QString ID = "0", QString CLIENTE = "", QString DOMICILIO = "", QString CIUDAD = "",
           QString RFC = "", QString AGENTE = "",
           QString CONDICIONES = "");

    void searchResult(QVector<QString>& items) const;
};


class Client_info : public Client
{
public:
    double m_TOTAL_NUM_BOXES;
    double m_DISCOUNT;

    // functions
    Client_info(QString ID = "0", QString CLIENTE = "",
                QString DOMICILIO = "", QString CIUDAD = "",
                QString RFC = "", QString AGENTE = "",
                QString CONDICIONES = "", double TOTAL_NUM_BOXES = 0., double DISCOUNT = 0.);
};

#endif // CLIENT_INFO_H
