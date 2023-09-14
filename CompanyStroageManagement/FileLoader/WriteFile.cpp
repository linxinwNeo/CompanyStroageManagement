#include "WriteFile.h"
#include "GlobalVars.h"
#include "Others/output_error_file.h"

/* write models to a models.txt file
 * for each model, we need to output its properties, more specifically, output the container's ID */
void WriteFile::Models2txt(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::Models2txt: couldn't create the file: " + path);
        return;
    }

    QTextStream out(&file);
    for(const auto& m : inventory.model_set){
        out << m->MODEL_CODE + split_item;
        out << m->DESCRIPTION_SPAN + split_item;
        out << m->DESCRIPTION_CN + split_item;
        out << QString::number(m->PRIZE) + split_item;
        out << QString::number(m->NUM_INIT_BOXES) + split_item;
        out << QString::number(m->NUM_SOLD_BOXES) + split_item;
        out << QString::number(m->NUM_LEFT_ITEMS) + split_item;
        out << QString::number(m->NUM_LEFT_BOXES) + split_item;
        out << QString::number(m->NUM_ITEMS_PER_BOX) + split_item;
        if(m->container.isNull()){
            out << "-1\n"; // if this model does not have a container, we put -1 to indicate
        }
        else{
            out << m->container->ID + "\n";
        }
    }
    file.close();
    return;
}


/* write the models to a xlsx file */
void WriteFile::Models2xlsx(const QString &path) const
{
    //TODO
}


/* write lists to a txt file */
void WriteFile::Lists2txt(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::Lists2txt: couldn't create the file: " + path);
        return;
    }

    QTextStream out(&file);

    // output total number of lists
    out << QString::number(lists.num_lists()) << "\n\n";

    for(const ListPtr& list : lists.lists){
        // output list id first
        out << list->id << split_item; // 0
        // output list create data and time
        out << list->date_created.toString("dd MMM yyyy") << split_item // 1
            << list->time_created.toString("hh:mm:ss") << split_item; // 2

        // output client_info
        out << list->client_info.CLIENTE << split_item // 3
            << list->client_info.DOMICILIO << split_item // 4
            << list->client_info.CIUDAD << split_item // 5
            << list->client_info.RFC << split_item // 6
            << list->client_info.AGENTE << split_item // 7
            << list->client_info.CONDICIONES << split_item // 8
            << list->client_info.TOTAL_NUM_BOXES << split_item // 9
            << list->client_info.DISCOUNT << split_item; // 10

        // output num of models in the list
        out << list->num_model_types() << "\n"; // 11
        
        for(EntryPtr entry : list->entryList.entries){
            out << entry->CLAVE << split_item // 0 modelCODE
                << entry->ContainerID << split_item // 1 container ID
                << entry->CAJA << split_item // 2 NUM_BOXES
                << entry->CANTIDAD << split_item // 3 TOTAL_NUM_ITEMS
                << entry->CANT_POR_CAJA << split_item // 4 NUM_ITEMS_PER_BOX
                << entry->Description_SPAN << split_item // 5 品名（西语）
                << entry->Description_CN << split_item // 6 品名（中文）
                << entry->PRECIO << split_item // 7 单价
                << entry->IMPORTE // 8 总价
                << "\n"; // 换行
        }
    }

    file.close();
}
