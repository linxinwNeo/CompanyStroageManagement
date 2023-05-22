#ifndef ENTRYLIST_H
#define ENTRYLIST_H

#include <vector>
#include <QDebug>
#include "entry.h"


class EntryList
{
public:
    // variables
    std::vector<Entry *> entries;

    // functions
    inline EntryList();
    inline ~EntryList();
    void clear_memory();

    void add_entry(Entry*);
    void remove_entry();
    void remove_entry(const UL);
    void update_entry(const UL, Entry*);
    inline UL num_entries() const;
    Entry *get_entry(const UL);

    UI size() const {return this->entries.size();};

    double subtotal() const;
};


EntryList::EntryList()
{
    this->entries.reserve(20);
}

EntryList::~EntryList()
{
    this->clear_memory();
}

UL EntryList::num_entries() const
{
    return this->entries.size();
}

#endif // ENTRYLIST_H
