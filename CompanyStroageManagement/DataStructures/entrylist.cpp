#include "DataStructures/entrylist.h"

void EntryList::clear_memory()
{
    for(Entry* e : this->entries){
        if(e) delete e;
    }

    this->entries.clear();
}

void EntryList::add_entry(Entry * to_be_added)
{
    if(to_be_added){
        this->entries.push_back(to_be_added);
    }
}

// remove the last entry
void EntryList::remove_entry()
{
    if(this->num_entries() != 0){
        auto entry = this->entries[ this->entries.size()-1 ];
        delete entry;
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

    auto entry = this->entries[ idx ];
    delete entry;

    this->entries.erase(this->entries.begin() + idx); // get this idx out from the vector
}


// update one entry
void EntryList::update_entry(const UL idx, Entry* e)
{
    // check if the idx is valid
    if( idx >= this->num_entries()-1 ) {
        qDebug() << "void EntryList::update_entry(const UL idx, Entry* e): idx invalid!";
        return;
    }

    if(this->entries[idx] != nullptr){
        delete this->entries[idx];
    }
    this->entries[idx] = e;
}

// return an entry at idx
Entry *EntryList::get_entry(const UL idx)
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
    UL subtotal = 0;
    for(auto entry : this->entries){
        subtotal += entry->IMPORTE;
    }
    return subtotal;
}
