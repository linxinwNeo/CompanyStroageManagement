#include "ClientManager.h"
#include "Others/write_error_file.h"
#include "FileLoader/WriteFile.h"

ClientManager::ClientManager() {
    this->clear();
    this->clients.reserve(1000);
}

ClientManager::~ClientManager() {

}

unsigned long ClientManager::num_clients() const
{
    return this->clients.size();
}

void ClientManager::reserve(const unsigned long size)
{
    this->clients.reserve(size);
}

// we allow replacements
bool ClientManager::add_client(QSharedPointer<Client> &client)
{
    if(client.isNull()){
        return false;
    }

    this->clients[client->m_ID] = client;

    WriteFile::Save_Clients();

    return true;
}

bool ClientManager::delete_client(const QString client_id_to_be_removed)
{
    if(this->clients.contains(client_id_to_be_removed)){
        this->clients.remove(client_id_to_be_removed);
         WriteFile::Save_Clients();
        return true;
    }
    else{
        return false;
    }
}


QSharedPointer<Client> ClientManager::get_client_by_ID(const QString clientID)
{
    if(this->clients.contains(clientID)){
        return this->clients[clientID];
    }
    return nullptr;
}

void ClientManager::get_clients_by_IDPrefix(const QString clientIDPrefix,
                                           QVector<QSharedPointer<Client> >& candidates){
    candidates.clear();
    candidates.reserve(50);


    for(QSharedPointer<Client>& client : this->clients){
        if(client.isNull()) continue;
        if(client->m_ID.startsWith(clientIDPrefix))
        {
            candidates.push_back(client);
        }
    }
}


void ClientManager::get_clients_by_NamePrefix(const QString clientName,
                                             QVector<QSharedPointer<Client> >& candidates)
{
    candidates.clear();
    candidates.reserve(50);

    const QString ClientNameUpperCase = clientName.toUpper();

    for(QSharedPointer<Client>& client : this->clients){
        if(client.isNull()) continue;
        if(client->m_clientName.toUpper().startsWith(ClientNameUpperCase)){
            candidates.push_back(client);
        }
    }
}


void ClientManager::clear()
{
    this->clients.clear();
}
