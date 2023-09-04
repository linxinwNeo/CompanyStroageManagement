#include "List.h"
#include "FileLoader/WriteFile.h"
#include "Others/output_error_file.h"

unsigned long int List::num_model_types() const
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


// make a list of strings that describes this list in a row of table
QVector<QString> List::describe_this_list() const
{
    QVector<QString> items(6);

    items[0] = QString::number(this->id);
    items[1] = this->date_created.toString("dd MMM yyyy");
    items[2] = this->time_created.toString("hh:mm:ss");
    items[3] = QString::number(this->total_num_boxes());
    double p1, p2;
    p1 = p2 = 0;
    this->total(p1, p2);
    items[4] = QString::number(p2);
    items[5] = this->client_info.CLIENTE;

    return items;
}


// return a unique id that is not used by any list so far
unsigned long int Lists::get_unique_id() const
{
    // we test integers from 1 to ULONG_MAX, 0 is intentionally left blank
    for(long long i = 1; i < ULONG_LONG_MAX; i++){
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

    this->lists.insert(list_2be_added->id, list_2be_added);
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
ListPtr Lists::get_list(unsigned long id)
{
    if(!this->lists.contains(id)) return nullptr;
    return this->lists[id];
}


// get the lists with their id begin with id_prefix
void Lists::get_list(QString id_prefix, QVector<ListPtr>& candidates)
{
    candidates.clear();
    candidates.reserve(this->num_lists()/9 + 10);

    // return if id_prefix is empty
    if(id_prefix.isEmpty()) return;

    // for each list in the database, we convert it to a string and testing
    for(ListPtr list : this->lists){
        QString cur_id = QString::number(list->id);
        if(cur_id.startsWith(id_prefix)){
            candidates.push_back(list);
        }
    }

    return;
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
