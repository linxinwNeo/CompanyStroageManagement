/* 这个 list 是清单的意思。
 * 一个清单会有多个Entry并由entryList控制
*/
#ifndef LIST_H
#define LIST_H

#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"
#include <QDate>
#include <QTime>

#define ListPtr QSharedPointer<List>
class List
{
public:
    Client_info client_info;
    EntryList entryList;
    QDate date_created;
    QTime time_created;
    unsigned long id;

    List() {};
    ~List() {};

    unsigned long num_model_types() const;
    void add_item(EntryPtr&);
    double total_num_boxes() const;

    void total(double& p1, double& p2) const;

    void return_models() const;

    QVector<QString> describe_this_list() const;
};


#define ListsPtr QSharedPointer<Lists>
class Lists
{
public:
    QHash<unsigned long, ListPtr> lists; // <id, list> pairs

    Lists() {};
    ~Lists() {};

    unsigned long get_unique_id() const;

    void add_list(ListPtr);
    ListPtr remove_list(unsigned long id);

    unsigned long num_lists() const;

    ListPtr get_list(unsigned long id);

    void get_list(QString id_prefix, QVector<ListPtr>& candidates, bool sorted);

    bool save_2_file(const bool save_path) const;

    void clear();

};

#endif // LIST_H
