#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include "DataStructures/List.h"

class ListManager
{
public:
    ListManager();
    ~ListManager();

    unsigned long max_id; // the largest id that is currently being used by a list

    void update_max_id();

    unsigned long get_unique_id() const;

    void create_list(ListPtr);
    bool delete_list(const QDateTime& creationDateTime);

    ListPtr get_list(const QDateTime& creationDateTime);

    void get_lists_by_listID_prefix(const QString id_prefix, QVector<ListPtr>& candidates, bool sorted);
    void get_lists_by_clientID_prefix(const QString id_prefix, QVector<ListPtr>& candidates, bool sorted);
    void get_lists_by_clientName_prefix(const QString name_prefix, QVector<ListPtr>& candidates, bool sorted);

    void clear();

};

#endif // LISTMANAGER_H
