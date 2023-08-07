#ifndef ENTRYLIST_H
#define ENTRYLIST_H

#include <QVector>
#include <QSharedPointer>
#include <QDebug>
#include "entry.h"


class EntryList
{
public:
    // variables
    QVector< QSharedPointer<Entry> > entries;

    // functions
    inline EntryList();
    inline ~EntryList();
    void clear_memory();

    void add_entry(QSharedPointer<Entry>& entry);
    void remove_entry();
    void remove_entry(const UL);
    void update_entry(const UL, QSharedPointer<Entry>& entry);
    inline UL num_entries() const;
    QSharedPointer<Entry> get_entry(const UL);

    double total_num_boxes() const;
};


EntryList::EntryList()
{
    this->entries.reserve(40);
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
