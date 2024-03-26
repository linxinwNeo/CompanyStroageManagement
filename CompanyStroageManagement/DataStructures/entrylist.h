#ifndef ENTRYLIST_H
#define ENTRYLIST_H

#include <QVector>
#include <QSharedPointer>
#include <QDebug>
#include "Entry.h"


class EntryList
{
public:
    // variables
    QVector< EntryPtr > entries;

    // functions
    inline EntryList();
    inline ~EntryList();
    void clear_memory();

    void add_entry(EntryPtr& entry);
    void remove_last_entry();
    void remove_entry(const unsigned long);
    inline unsigned long num_entries() const;
    EntryPtr get_entry(const unsigned long);

    bool has_Model(const QString &ModelCode, const QString &ContainerID) const;

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

unsigned long EntryList::num_entries() const
{
    return this->entries.size();
}

#endif // ENTRYLIST_H
