#include "DataStructures/entrylist.h"

void EntryList::clear_memory()
{
    this->entries.clear();
}

void EntryList::add_entry(QSharedPointer<Entry>& entry_to_be_added)
{
    if(!entry_to_be_added.isNull()){
        this->entries.push_back(entry_to_be_added);
    }
}

// remove the last entry
void EntryList::remove_entry()
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


// update one entry
void EntryList::update_entry(const UL idx, QSharedPointer<Entry>& e)
{
    // check if the idx is valid
    if( idx >= this->num_entries()-1 ) {
        qDebug() << "void EntryList::update_entry(const UL idx, Entry* e): idx invalid!";
        return;
    }

    this->entries[idx] = e;
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

// calculate the subtotal
double EntryList::subtotal() const
{
    double subtotal = 0;
    for(auto entry : this->entries){
        subtotal += entry->IMPORTE;
    }
    return subtotal;
}
