/* 这个 list 是清单的意思。
 * 一个清单会有多个Entry并由entryList控制
*/
#ifndef LIST_H
#define LIST_H

#include "DataStructures/client_info.h"
#include "DataStructures/Entrylist.h"
#include <QDateTime>

#define ListPtr QSharedPointer<List>
class List
{
public:
    Client_info client_info;
    EntryList entryList;
    QSharedPointer<QDateTime> datetime_created = nullptr;
    unsigned long id;

    List() {};
    ~List() {};

    unsigned long num_model_types() const;
    void add_item(EntryPtr&);
    double total_num_boxes() const;

    void total(double& p1, double& p2) const;
    
    void AddBack_Models() const;

    QVector<QString> describe_this_list() const;
};


#endif // LIST_H
