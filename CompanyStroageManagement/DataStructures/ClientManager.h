#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include "DataStructures/client_info.h"

class ClientManager
{
public:
    ClientManager();
    ~ClientManager();

    QHash< QString, QSharedPointer<Client> > clients;

    unsigned long num_clients() const;

    void reserve(const unsigned long);

    bool add_client(QSharedPointer<Client>& client);
    bool delete_client(const QString client_id_to_be_removed);

    QSharedPointer<Client> get_client_by_ID(const QString clientID);

    void get_clients_by_IDPrefix(const QString clientIDPrefix,
                                QVector<QSharedPointer<Client> >& candidates);
    void get_clients_by_NamePrefix(const QString clientName,
                            QVector<QSharedPointer<Client> >& candidates);

    void clear();
};

#endif // CLIENTMANAGER_H
