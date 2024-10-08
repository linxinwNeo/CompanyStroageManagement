#include "DataStructures/Entrylist.h"
#include "Algorithm/QuickSort.h"
#include "Others/write_error_file.h"

void EntryList::clear_memory()
{
    this->entries.clear();
}


void EntryList::add_entry(QSharedPointer<Entry>& entry_to_be_added)
{
    if(entry_to_be_added.isNull()) return;

    this->entries.push_back(entry_to_be_added);

    // sort the list by their MODELCODE/CLAVE
    QuickSorts::QuickSort(this->entries);
}


// remove the last entry
void EntryList::remove_last_entry()
{
    if(this->num_entries() != 0){
        this->entries.pop_back();
    }
}


// remove an arbitrary entry if exists
void EntryList::remove_entry(const unsigned long idx)
{
    // check if the idx is valid
    if( !(idx <= this->num_entries() - 1) ) {
        write_error_file("void EntryList::remove_entry(const UL idx): idx" +
                         QString::number(idx) +" invalid!");
        return;
    }

    this->entries.erase(this->entries.begin() + idx); // get this element of idx out from the vector
}


// return an entry at idx
QSharedPointer<Entry> EntryList::get_entry(const unsigned long idx)
{
    // check if the idx is valid
    if( idx >= this->num_entries() ) {
        QString errorMessage = "Entry *EntryList::get_entry(const UL idx): idx " +
                              QString::number(idx) +
                              " invalid!";

        write_error_file(errorMessage);
        return nullptr;
    }

    return this->entries[idx];
}


// iterate all entries, check if the corresponding model exists
bool EntryList::has_Model(const QString &ModelCode, const QString &ContainerID) const
{
    QString trimmed_ID = ContainerID.trimmed();
    for(const EntryPtr& entry : this->entries){
        if(ModelCode == entry->MODEL_CODE){
            if(trimmed_ID == entry->ContainerID.trimmed()) return true;
        }
    }

    return false;
}


double EntryList::total_num_boxes() const
{
    double sum = 0;
    for(const QSharedPointer<Entry>& entry : this->entries){
        sum += entry->num_boxes();
    }
    return sum;
}
