#include "List.h"
#include "Algorithm/QuickSort.h"
#include "FileLoader/WriteFile.h"
#include "GlobalVars.h"
#include "Others/write_error_file.h"

unsigned long List::num_model_types() const
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
        p1 += (entry->TOTAL);
    }

    p2 = (p1 * (1. - this->client_info.m_DISCOUNT) );
}


// 将该清单内的所有货物加回库存
void List::AddBack_Models() const
{
    for(const EntryPtr& entry : this->entryList.entries){
        // 找到对应货号的库存
        ModelPtr model = inventory.get_Model(entry->MODEL_CODE, entry->ContainerID);
        ContainerPtr container = inventory.get_container(entry->ContainerID);
        if(model.isNull()){
            /* the model doesn't exist in inventory, this is usually because the model is deleted
             * after this list has been created, we need to create this model */
            model = ModelPtr (new Model(entry->MODEL_CODE, // 货号
                                       entry->Description_SPAN, // 品名西语
                                       entry->Description_CN, // 品名中文
                                       entry->PRICE_PER_PIECE, // 单价
                                       entry->NUM_PIECES, // 进货个数
                                       0, // 已售个数
                                       entry->NUM_PIECES_PER_BOX)); // 每箱个数
            inventory.add_new_Model(model);

            /* if the container of this model also doesn't exist, we need to create one */
            if(container.isNull() && !entry->ContainerID.isEmpty()){
                /* the container for this model does not exist, we need to create a new container */
                container = ContainerPtr (new Container(entry->ContainerID));
                container->add_model(model);
                inventory.add_Container(container);
                model->m_Container = container;
            }

            continue;
        }

        // the model exists in our inventory, so we can add the model back directly
        // the addBack function will handle the case that after adding back the num of boxes exceed the max
        model->AddBack(entry->NUM_PIECES);
    }
}


// make a list of strings that describes this list in a row of table
QVector<QString> List::describe_this_list() const
{
    QVector<QString> items(6);

    items[0] = QString::number(this->id);
    if(this->datetime_created.isNull()){
        items[1] = QDateTime().currentDateTime().toString(GlobalVars::DateTimeFormat);
    }
    else{
        items[1] = this->datetime_created->toString(GlobalVars::DateTimeFormat);
    }


    items[2] = GlobalVars::locale.toString(this->total_num_boxes(), 'f', 2);
    double p1, p2;
    p1 = p2 = 0;
    this->total(p1, p2);
    items[3] = GlobalVars::locale.toString(p2, 'f', 2);
    items[4] = this->client_info.m_clientName;

    return items;
}

