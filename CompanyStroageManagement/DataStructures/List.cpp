#include "List.h"
#include "FileLoader/WriteFile.h"
#include "Others/output_error_file.h"

unsigned long int List::num_items() const
{
    return this->itemList.num_entries();
}


void List::add_item(QSharedPointer<Entry> & new_entry)
{
    this->itemList.add_entry(new_entry);
}


double List::total_num_boxes() const
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


// return a unique id that is not used by any list so far
unsigned long int Lists::get_unique_id() const
{
    // we test integers from 0 to ULONG_MAX
    for(long long i = 0; i < ULONG_LONG_MAX; i++){
        if(lists.contains(i)) continue;
        return i;
    }


    write_error_file("Lists::get_unique_id() couldn't get a valid id");
    exit(-1);
}


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


// write the list information in a file
void Lists::save_2_file() const
{
    WriteFile wf;

    wf.Lists2txt("lists.txt");
}


void Lists::clear()
{
    this->lists.clear();

}
