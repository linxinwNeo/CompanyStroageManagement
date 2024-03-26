#include "WriteFile.h"
#include "CN_Strings.h"
#include "GlobalVars.h"
#include "Others/write_error_file.h"
#include "header/xlsxdocument.h"
#include <QDir>
#include "IO_Base.h"

/* write models to a models.txt file
 * for each model, we need to output its properties, more specifically, output the container's ID */

WriteFile::WriteFile()
{

}


/* 2/24/2024
 * Automatically save inventory file to the path stored in last_inventory_path
 */
bool WriteFile::SaveInventoryAuto(const bool save_path)
{
    if(last_inventory_path.endsWith(".xlsx"))
        return WriteFile::Inventory2Xlsx(last_inventory_path, save_path);
    else if(last_inventory_path.endsWith(".txt")){
        return WriteFile::Inventory2Txt(last_inventory_path, save_path);
    }
    else{
        return false;
    }
}

/* Automatically save inventory file to the path stored in last_inventory_path
 * <save_path> indicates if we want to save the path to last_inventory_path
 * We dont save path if save is not successful
 */
bool WriteFile::SaveInventoryAuto(const QString &path, const bool save_path)
{
    if(path.endsWith(".xlsx"))
        return WriteFile::Inventory2Xlsx(path, save_path);
    else if(path.endsWith(".txt")){
        return WriteFile::Inventory2Txt(path, save_path);
    }
    else{
        return false;
    }
}

bool WriteFile::Inventory2Txt(const QString &path, const bool save_path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::Inventory2Txt: couldn't create the file: " + path);
        return false;
    }

    QTextStream out(&file);
    // write the number of models
    out << "% " << inventory.num_models() << split_item << " \n";
    // write the name for each column
    out << QString("% 货号/MODEL_CODE ") +
            "集装箱号/CONTAINER_ID " +
            "品名（中文）/DESCRIPTION_CN " +
            "品名（西语）/DESCRIPTION_SPAN " +
            "进货个数/NUM_INITIAL_PIECES " +
            "已售个数/NUM_SOLD_PIECES " +
            "剩余个数/NUM_LEFT_PIECES " +
            "每箱个数/NUM_PIECES_PER_BOX " +
            "单价/PRIZE_PER_PIECE " +
            "上次修改时间/TIME_MODIFIED \n";

    for(const QSharedPointer<Model>& m : inventory.model_set){
        out << m->MODEL_CODE + split_item; // 1. 货号/MODEL_CODE
        if(m->container.isNull()){  // 2. 集装箱号/CONTAINER_ID
            out << "-1\n"; // if this model does not have a container, we put -1 to indicate
        }
        else{
            out << m->container->ID + "\n";
        }

        out << m->DESCRIPTION_CN + split_item; // 3. 品名（中文）
        out << m->DESCRIPTION_SPAN + split_item; // 4. 品名（西语）

        out << QString::number(m->NUM_INIT_PIECES) + split_item; // 5. 进货个数/NUM_INITIAL_PIECES
        out << QString::number(m->NUM_SOLD_PIECES) + split_item; // 6. 已售个数/NUM_SOLD_PIECES
        out << QString::number(m->NUM_LEFT_PIECES) + split_item; // 7. 剩余个数/NUM_LEFT_PIECES
        out << QString::number(m->NUM_PIECES_PER_BOX) + split_item;  // 8. 每箱个数/NUM_PIECES_PER_BOX
        out << QString::number(m->PRIZE) + split_item; // 9. 单价/PRIZE_PER_PIECE

        if(m->last_time_modified.isNull()){ // 10. 上次修改时间/TIME_MODIFIED
            const QDateTime currentDateTime = QDateTime::currentDateTime();
            out << currentDateTime.toString(DateTimeFormat) + split_item + "\n";
        }
        else{
            out << m->last_time_modified->toString(DateTimeFormat) + split_item + "\n";
        }

    }
    file.close();

    if(save_path){
        last_inventory_path = path;
    }

    return true;
}


/* write the models to a xlsx file */
bool WriteFile::Inventory2Xlsx(const QString &path, const bool save_path)
{
    using namespace QXlsx;

    // create a new file
    QXlsx::Document xlsx;

    unsigned int long row = 1;
    unsigned int long col = 1;

    // write the info about each column
    xlsx.write(row, col++, "货号/MODEL_CODE");
    xlsx.write(row, col++, "集装箱号/CONTAINER_ID");
    xlsx.write(row, col++, "品名（中文）/DESCRIPTION_CN");
    xlsx.write(row, col++, "品名（西语）/DESCRIPTION_SPAN");

    xlsx.write(row, col++, "进货个数/NUM_INITIAL_PIECES");
    xlsx.write(row, col++, "已售个数/NUM_SOLD_PIECES");
    xlsx.write(row, col++, "剩余个数/NUM_LEFT_PIECES");

    xlsx.write(row, col++, "每箱个数/NUM_PIECES_PER_BOX");
    xlsx.write(row, col++, "单价/PRIZE_PER_PIECE");
    xlsx.write(row, col++, "上次修改时间/TIME_MODIFIED");

    row ++;

    QVector<ModelPtr> models;
    inventory.modelSet_2_Vector(models); // also sorts the models

    // write models to it
    for(const ModelPtr& model : models){
        col = 1;
        xlsx.write(row, col++, model->MODEL_CODE); // 1. 货号/MODEL_CODE

        if(!model->container.isNull()) xlsx.write(row, col++, model->container->ID); // 2. 集装箱号/CONTAINER_ID
        else xlsx.write(row, col++, "-1"); // if this model does not have a container, we put -1 to indicate

        xlsx.write(row, col++, model->DESCRIPTION_CN); // 3. 品名（中文）/DESCRIPTION_CN
        xlsx.write(row, col++, model->DESCRIPTION_SPAN); // 4. 品名（西语）/DESCRIPTION_SPAN

        // 需要cast到unsigned long long才不会报错
        xlsx.write(row, col++, (unsigned long long)model->NUM_INIT_PIECES); // 5. 进货个数/NUM_INITIAL_PIECES
        xlsx.write(row, col++, (unsigned long long)model->NUM_SOLD_PIECES); // 6. 已售个数/NUM_SOLD_PIECES
        xlsx.write(row, col++, (unsigned long long)model->NUM_LEFT_PIECES); // 7. 剩余个数/NUM_LEFT_PIECES

        xlsx.write(row, col++, QString::number(model->NUM_PIECES_PER_BOX)); // 8. 每箱个数/NUM_PIECES_PER_BOX
        xlsx.write(row, col++, model->PRIZE); // 9. 单价/PRIZE_PER_PIECE

        if(model->last_time_modified.isNull()){ // 10. 上次修改时间
            QDateTime currentDateTime = QDateTime::currentDateTime();
            xlsx.write(row, col++, currentDateTime.toString(DateTimeFormat));
        }
        else{
            xlsx.write(row, col++, model->last_time_modified->toString(DateTimeFormat));
        }

        row++;
    }

    // Save the file
    const bool success = xlsx.saveAs(path);
    if(!success){
        return false;
    }

    if(save_path){
        last_inventory_path = path;
    }
    return true;
}

bool WriteFile::Lists2txt(const bool save_path)
{
    return Lists2txt(last_lists_path, save_path);
}


/* write lists to a txt file */
bool WriteFile::Lists2txt(const QString &path, const bool save_path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::Lists2txt: couldn't create the file: " + path + " \n");
        return false;
    }

    QTextStream out(&file);

    // output total number of lists
    out << QString::number(lists.num_lists()) << "\n\n";

    for(const ListPtr& list : lists.lists){
        // output list id first
        out << list->id << split_item; // 0
        // output list creation datatime
        if(list->datetime_created.isNull()){ // if no date time, put current time
            list->datetime_created = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());
        }
        out << list->datetime_created->toString(DateTimeFormat) << split_item;

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
        
        for(EntryPtr& entry : list->entryList.entries){
            out << entry->CLAVE << split_item // 0. 货号
                << entry->ContainerID << split_item // 1. 集装箱号
                << entry->num_boxes() << split_item // 2. 箱数
                << entry->CANTIDAD << split_item // 3. 总个数
                << entry->CANT_POR_CAJA << split_item // 4 每箱几个
                << entry->Description_SPAN << split_item // 5 品名（西语）
                << entry->Description_CN << split_item // 6 品名（中文）
                << entry->PRECIO << split_item // 7 单价
                << entry->IMPORTE // 8 总价
                << "\n"; // 换行
        }
    }

    file.close();

    if(save_path){
        last_inventory_path = path;
    }

    return true;
}


// we update the BackUpDate
bool WriteFile::update_BackUpDate()
{
    QFile file(BackUP_FileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        QDateTime currentDateTime = QDateTime::currentDateTime();

        stream << currentDateTime.toString(DateTimeFormat);

        // Close the file when done
        file.close();

        return true;
    }
    else{
        // write error message to
        write_error_file(UNABLE_TO_SAVE_BACKUP_DATA_MSG_CN);
        return false;
    }
}


// save the back up files in a folder
bool WriteFile::save_BackUp_files(const bool save_path)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString folderName = currentDateTime.toString(DateTimeFormat);
    QDir timeDir;

    // Create the subfolder named by datetime
    QString path = BackUP_DirName + "/" + folderName;
    bool success = timeDir.mkpath(path);
    if(!success){
        write_error_file("WriteFile::save_BackUp_files: couldn't create the path: " + path + " \n");
        return false;
    }

    QString path_to_list_file = BackUP_DirName + "/" + folderName + "/lists.txt";
    if( !Lists2txt(path_to_list_file, save_path) )
        return false;

    QString path_to_inventory_file = BackUP_DirName + "/" + folderName + "/inventory.xlsx";
    if( !SaveInventoryAuto(path_to_inventory_file, save_path) )
        return false;

    return true;
}


// save the settings to the file
bool WriteFile::save_settings_file()
{
    QFile file(Settings_FileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::save_settings_file: couldn't create the file: " + Settings_FileName);
        return false;
    }

    QTextStream out(&file);

    // write language option
    out << language_option << " \n"; // output language setting

    // write last time opened inventory file path
    out << last_inventory_path << " \n"; // output inventory path

    // write last time opened lists file path
    out << last_lists_path << " \n"; // output lists path

    file.close();
    return true;
}
