#include "List.h"

List::List()
{

}

List::~List()
{

}

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
