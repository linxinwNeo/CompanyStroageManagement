#ifndef LIST_H
#define LIST_H

#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"

#define ListPtr QSharedPointer<List>
class List
{
public:
    Client_info client_info;
    EntryList itemList;

    List();
    ~List();

    unsigned int num_items() const;
    void add_item(EntryPtr&);
    unsigned int total_num_boxes() const;

    void total(double& p1, double& p2) const;
};

#endif // LIST_H
