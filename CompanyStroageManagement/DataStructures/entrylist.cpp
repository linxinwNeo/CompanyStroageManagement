#include "DataStructures/entrylist.h"
#include "Algorithm/QuickSort.h"
#include "Others/handle_containerID.h"

void EntryList::clear_memory()
{
    this->entries.clear();
}


void EntryList::add_entry(QSharedPointer<Entry>& entry_to_be_added)
{
    if(entry_to_be_added.isNull()) return;

    this->entries.append(entry_to_be_added);

    // sort the list by their MODELCODE/CLAVE
    QuickSorts QS;
    QS.QuickSort(this->entries);
}


// remove the last entry
void EntryList::remove_last_entry()
{
    if(this->num_entries() != 0){
        this->entries.pop_back();
    }
}


// remove an arbitrary entry if exists
void EntryList::remove_entry(const UL idx)
{
    // check if the idx is valid
    if( !(idx <= this->num_entries() - 1) ) {
        qDebug() << "void EntryList::remove_entry(const UL idx): idx invalid!";
        return;
    }

    this->entries.erase(this->entries.begin() + idx); // get this idx out from the vector
}


// return an entry at idx
QSharedPointer<Entry> EntryList::get_entry(const UL idx)
{
    // check if the idx is valid
    if( idx >= this->num_entries() ) {
        qDebug() << "Entry *EntryList::get_entry(const UL idx): idx invalid!";
        return nullptr;
    }

    return this->entries[idx];
}


// iterate all entries, check if the corresponding model exists
bool EntryList::has_Model(const QString &ModelCode, const QString &ContainerID) const
{
    QString container_2Be_Compared = ContainerID;
    handle_ContainerID(container_2Be_Compared);

    for(const EntryPtr& entry : this->entries){
        if(ModelCode == entry->CLAVE){
            QString entryContainer = entry->ContainerID;
            handle_ContainerID(entryContainer);
            if(container_2Be_Compared == entryContainer) return true;
        }
    }

    return false;
}


double EntryList::total_num_boxes() const
{
    double sum = 0;
    for(auto entry : this->entries){
        sum += entry->CAJA;
    }
    return sum;
}
