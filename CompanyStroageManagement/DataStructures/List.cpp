#include "List.h"
#include "Algorithm/QuickSort.h"
#include "FileLoader/WriteFile.h"
#include "GlobalVars.h"
#include "Others/output_error_file.h"

unsigned long int List::num_model_types() const
{
    return this->entryList.num_entries();
}


void List::add_item(QSharedPointer<Entry> & new_entry)
{
    this->entryList.add_entry(new_entry);
}


double List::total_num_boxes() const
{
    return this->entryList.total_num_boxes();
}


// compute the total prize
// p1 is the value without discount
// p2 is the value with discount
void List::total(double &p1, double &p2) const
{
    p1 = 0;

    for(auto& entry : this->entryList.entries){
        p1 += (entry->IMPORTE);
    }

    p2 = (p1 * (1. - this->client_info.DISCOUNT) );
}


// 将该清单内的所有货物加回库存
void List::return_models() const
{
    for(const EntryPtr& entry : this->entryList.entries){
        // 找到对应货号的库存
        ModelPtr model = inventory.get_Model(entry->CLAVE, entry->ContainerID);
        ContainerPtr container = inventory.get_container(entry->ContainerID);
        if(model.isNull()){
            /* the model doesn't exist in inventory, this is usually because the model is deleted
             * after this list has been created, we need to create this model */
            model = ModelPtr (new Model(entry->CLAVE, entry->Description_SPAN, entry->Description_CN,
                                       entry->PRECIO, entry->CAJA, 0, entry->CAJA,
                                       entry->CANTIDAD, entry->CANT_POR_CAJA));
            inventory.add_Model(model);

            /* if the container of this model also doesn't exist, we need to create one */
            if(container.isNull() && !entry->ContainerID.isEmpty()){
                /* the container for this model does not exist, we need to create a new container */
                container = ContainerPtr (new Container(entry->ContainerID));
                container->add_model(model);
                inventory.add_Container(container);
                model->container = container;
            }

            continue;
        }

        // the model is not null, so we can add the model back directly
        // the addBack function will handle the case that after adding back the num of boxes exceed the max
        model->addBack_items(entry->CANTIDAD);
    }
}


// make a list of strings that describes this list in a row of table
QVector<QString> List::describe_this_list() const
{
    QVector<QString> items(6);

    items[0] = QString::number(this->id);
    items[1] = this->date_created.toString("dd MMM yyyy");
    items[2] = this->time_created.toString("hh:mm:ss");
    items[3] = QString::number(this->total_num_boxes(), 'f', 2);
    double p1, p2;
    p1 = p2 = 0;
    this->total(p1, p2);
    items[4] = QString::number(p2, 'f', 2);
    items[5] = this->client_info.CLIENTE;

    return items;
}


// return a unique id that is not used by any list so far
unsigned long int Lists::get_unique_id() const
{
    // we test integers from 1 to ULONG_MAX, 0 is intentionally left blank
    for(unsigned long long i = 1; i < ULONG_LONG_MAX; i++){
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
// if prefix is empty, we return all lists
void Lists::get_list(QString id_prefix, QVector<ListPtr>& candidates, bool sorted)
{
    candidates.clear();
    candidates.reserve(this->num_lists()/9 + 10);

    // return if id_prefix is empty
    if(id_prefix.isEmpty()){
        for(ListPtr list : this->lists){
            candidates.push_back(list);
        }
    }
    else{
        // for each list in the database, we convert it to a string and testing
        for(ListPtr list : this->lists){
            QString cur_id = QString::number(list->id);
            if(cur_id.startsWith(id_prefix)){
                candidates.push_back(list);
            }
        }
    }

    // sort the vector if needed
    if(sorted){
        QuickSorts QS;
        QS.QuickSort(candidates);
    }

    return;
}


// write the list information in a file
bool Lists::save_2_file(const bool save_path) const
{
    return WriteFile::Lists2txt(save_path);
}


void Lists::clear()
{
    this->lists.clear();

}
