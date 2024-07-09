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
    void delete_list(const unsigned long id);

    ListPtr get_list(const unsigned long id);

    void get_lists(const QString id_prefix, QVector<ListPtr>& candidates, bool sorted);

    void clear();

};

#endif // LISTMANAGER_H
