#ifndef LIST_H
#define LIST_H

#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"
#include <QDate>
#include <QTime>

#define ListPtr QSharedPointer<List>
class List
{
public:
    Client_info client_info;
    EntryList itemList;
    QDate date_created;
    QTime time_created;
    unsigned long int id;

    List() {};
    ~List() {};

    unsigned long int num_items() const;
    void add_item(EntryPtr&);
    double total_num_boxes() const;

    void total(double& p1, double& p2) const;
};


#define ListsPtr QSharedPointer<Lists>
class Lists
{
public:
    QHash<unsigned long int, ListPtr> lists; // <id, list> pairs

    Lists() {};
    ~Lists() {};

    unsigned long int get_unique_id() const;

    void add_list(ListPtr);
    ListPtr remove_list(unsigned long int id);

    unsigned long int num_lists() const;

    ListPtr get_list(unsigned long int id);

    void save_2_file() const;
};

#endif // LIST_H
