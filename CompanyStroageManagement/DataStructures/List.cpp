#include "List.h"

unsigned int List::num_items() const
{
    return this->itemList.num_entries();
}


void List::add_item(QSharedPointer<Entry> & new_entry)
{
    this->itemList.add_entry(new_entry);
}


unsigned int List::total_num_boxes() const
{
    return this->itemList.total_num_boxes();
}


// compute the total prize
// p1 is the value without discount
// p2 is the value with discount
void List::total(double &p1, double &p2) const
{
    p1 = 0;

    for(auto& entry : this->itemList.entries){
        p1 += (entry->IMPORTE);
    }

    p2 = (p1 * (1. - this->client_info.DISCOUNT) );
}


// add the list, assign id
void Lists::add_list(ListPtr list_2be_added)
{
    if(list_2be_added.isNull()){
        qDebug() << "Lists::add_list: trying to add a null list";
        return;
    }

    unsigned long int id = this->num_lists();
    list_2be_added->id = id;
    this->lists[id] = list_2be_added;
}


// remove the list with id and return the corresponding list, null if not exists
ListPtr Lists::remove_list(unsigned long int id)
{
    if(!lists.contains(id)) return nullptr;

    ListPtr list = lists[id];
    lists.remove(id);

    return list;
}


unsigned long Lists::num_lists() const
{
    return this->lists.size();
}


// get the reference to the list with specified id
QSharedPointer<List> Lists::get_list(unsigned long id)
{
    if(!this->lists.contains(id)) return nullptr;
    return this->lists[id];
}
