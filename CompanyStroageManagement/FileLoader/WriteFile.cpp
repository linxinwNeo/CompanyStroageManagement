#include "WriteFile.h"
#include "LanguageStrings.h"
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


// 保存一个list文件
bool WriteFile::Save_List(const ListPtr list){
    // now, output the list content
    QString fileName = QString::number(list->id);
    QDir DirMaker;

    bool success = DirMaker.mkpath("./" + GlobalVars::Lists_DirName);
    if(!success){
        write_error_file("WriteFile::Save_list: couldn't create the folder: ./" + GlobalVars::Lists_DirName);
        return false;
    }

    QString path_to_list_file = "./" + GlobalVars::Lists_DirName + "/" + fileName + ".txt";

    QFile file(path_to_list_file);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        write_error_file("WriteFile::Save_list: couldn't create the file: " + path_to_list_file + " \n");
        return false;
    }

    QTextStream out(&file);

    out << list->id << split_item;         // 输出清单号

    // 1. 输出创建时间和日期
    if(list->datetime_created.isNull()){ // if no date time, put current time
        list->datetime_created = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());
    }
    out << list->datetime_created->toString(GlobalVars::DateTimeFormat) << split_item;

    // output client_info
    out << list->client_info.m_ID << split_item // 2
        << list->client_info.m_clientName << split_item // 3
        << list->client_info.m_DOMICILIO << split_item // 4
        << list->client_info.m_CIUDAD << split_item // 5
        << list->client_info.m_RFC << split_item // 6
        << list->client_info.m_AGENTE << split_item // 7
        << list->client_info.m_CONDICIONES << split_item // 8
        << list->client_info.m_TOTAL_NUM_BOXES << split_item // 9
        << list->client_info.m_DISCOUNT << split_item; // 10

    // output num of models in the list
    out << list->num_model_types() << "\n"; // 11

    for(EntryPtr& entry : list->entryList.entries){
        out << entry->MODEL_CODE << split_item // 0. 货号
            << entry->ContainerID << split_item // 1. 集装箱号
            << entry->NUM_PIECES << split_item // 2. 总个数
            << entry->NUM_PIECES_PER_BOX << split_item // 3. 每箱几个
            << entry->Description_SPAN << split_item // 4. 品名（西语）
            << entry->Description_CN << split_item // 5. 品名（中文）
            << entry->PRICE_PER_PIECE << split_item // 6. 单价
            << entry->TOTAL // 7. 总价
            << "\n";
    }

    file.close();

    return true;
}


bool WriteFile::Save_Clients()
{
    QString path_to_clients_file = "./" + GlobalVars::Clients_FileName;
    QFile file(path_to_clients_file);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        write_error_file("WriteFile::Save_Clients: couldn't create the file: " + path_to_clients_file + " \n");
        return false;
    }

    QTextStream out(&file);

    out << clientManager.num_clients() << "\n"; // 客户数量

    for(const QSharedPointer<Client>& client : clientManager.clients){
        // output client
        out << client->m_ID << split_item         // 输出客户号码
            << client->m_clientName << split_item // 客户名
            << client->m_DOMICILIO << split_item // 3
            << client->m_CIUDAD << split_item // 4
            << client->m_RFC << split_item // 5
            << client->m_AGENTE << split_item // 6
            << client->m_CONDICIONES // 7
            << "\n";
    }

    file.close();

    return true;
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
            "每箱个数/NUM_PIECES_PER_BOX " +
            "单价/PRIZE_PER_PIECE " +
            "上次修改时间/TIME_MODIFIED" +
            "创建时间/TIME_CREATED \n";

    for(const QSharedPointer<Model>& m : inventory.model_set){
        out << m->m_MODEL_CODE + split_item; // 0. 货号/MODEL_CODE
        if(m->m_Container.isNull()){  // 1. 集装箱号/CONTAINER_ID
            out << "-1" + split_item; // if this model does not have a container, we put -1 to indicate
        }
        else{
            out << m->m_Container->ID + "" + split_item;
        }

        out << m->m_DESCRIPTION_CN + split_item; // 2. 品名（中文）
        out << m->m_DESCRIPTION_SPAN + split_item; // 3. 品名（西语）

        out << QString::number(m->m_NUM_INIT_PIECES) + split_item; // 4. 进货个数/NUM_INITIAL_PIECES
        out << QString::number(m->m_NUM_SOLD_PIECES) + split_item; // 5. 已售个数/NUM_SOLD_PIECES
        out << QString::number(m->m_NUM_PIECES_PER_BOX) + split_item;  // 7. 每箱个数/NUM_PIECES_PER_BOX
        out << QString::number(m->m_PRICE) + split_item; // 8. 单价/PRIZE_PER_PIECE

        if(m->m_last_time_modified.isNull()){ // 9. 上次修改时间/TIME_MODIFIED
            const QDateTime currentDateTime = QDateTime::currentDateTime();
            out << currentDateTime.toString(GlobalVars::DateTimeFormat) + split_item;
        }
        else{
            out << m->m_last_time_modified->toString(GlobalVars::DateTimeFormat) + split_item;
        }

        if(m->m_time_created.isNull()){ // 10. 创建时间
            const QDateTime currentDateTime = QDateTime::currentDateTime();
            out << currentDateTime.toString(GlobalVars::DateTimeFormat) + split_item + "\n";
        }
        else{
            out << m->m_time_created->toString(GlobalVars::DateTimeFormat) + split_item + "\n";
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

    xlsx.write(row, col++, "每箱个数/NUM_PIECES_PER_BOX");
    xlsx.write(row, col++, "单价/PRIZE_PER_PIECE");
    xlsx.write(row, col++, "上次修改时间/TIME_MODIFIED");
    xlsx.write(row, col++, "创建时间/TIME_CREATED");

    row ++;

    QVector<ModelPtr> models;
    inventory.modelSet_2_Vector(models); // also sorts the models

    // write models to it
    for(const ModelPtr& model : models){
        col = 1;
        xlsx.write(row, col++, model->m_MODEL_CODE); // 0. 货号/MODEL_CODE

        if(!model->m_Container.isNull()) xlsx.write(row, col++, model->m_Container->ID); // 1. 集装箱号/CONTAINER_ID
        else xlsx.write(row, col++, "-1"); // if this model does not have a container, we put -1 to indicate

        xlsx.write(row, col++, model->m_DESCRIPTION_CN); // 2. 品名（中文）/DESCRIPTION_CN
        xlsx.write(row, col++, model->m_DESCRIPTION_SPAN); // 3. 品名（西语）/DESCRIPTION_SPAN

        xlsx.write(row, col++, GlobalVars::locale.toString(model->m_NUM_INIT_PIECES)); // 4. 进货个数/NUM_INITIAL_PIECES
        xlsx.write(row, col++, GlobalVars::locale.toString(model->m_NUM_SOLD_PIECES)); // 5. 已售个数/NUM_SOLD_PIECES

        xlsx.write(row, col++, GlobalVars::locale.toString(model->m_NUM_PIECES_PER_BOX)); // 6. 每箱个数/NUM_PIECES_PER_BOX
        xlsx.write(row, col++, GlobalVars::locale.toString(model->m_PRICE, 'f', 2)); // 7. 单价/PRIZE_PER_PIECE

        if(model->m_last_time_modified.isNull()){ // 8. 上次修改时间
            QDateTime currentDateTime = QDateTime::currentDateTime();
            xlsx.write(row, col++, currentDateTime.toString(GlobalVars::DateTimeFormat));
        }
        else{
            xlsx.write(row, col++, model->m_last_time_modified->toString(GlobalVars::DateTimeFormat));
        }

        if(model->m_time_created.isNull()){ // 9. 创建时间
            QDateTime currentDateTime = QDateTime::currentDateTime();
            xlsx.write(row, col++, currentDateTime.toString(GlobalVars::DateTimeFormat));
        }
        else{
            xlsx.write(row, col++, model->m_time_created->toString(GlobalVars::DateTimeFormat));
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

// we update the BackUpDate
bool WriteFile::update_BackUpDate()
{
    QFile file(GlobalVars::BackUP_FileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        QDateTime currentDateTime = QDateTime::currentDateTime();

        stream << currentDateTime.toString(GlobalVars::DateTimeFormat_for_backup_file);

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
    QString folderName = currentDateTime.toString(GlobalVars::DateTimeFormat_for_backup_file);
    QDir timeDir;

    // Create the subfolder named by datetime
    QString path = GlobalVars::BackUP_DirName + "/" + folderName;
    bool success = timeDir.mkpath(path);
    if(!success){
        write_error_file("WriteFile::save_BackUp_files: couldn't create the path: " + path);
        return false;
    }

    QString path_to_inventory_file = GlobalVars::BackUP_DirName + "/" + folderName + "/inventory.xlsx";
    if( !SaveInventoryAuto(path_to_inventory_file, save_path) )
        return false;

    return true;
}


// save the settings to the file
bool WriteFile::save_settings_file()
{
    QFile file(GlobalVars::Settings_FileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::save_settings_file: couldn't create the file: " + GlobalVars::Settings_FileName);
        return false;
    }

    QTextStream out(&file);

    // write language option
    out << GlobalVars::language_option << " \n"; // output language setting

    // write last time opened inventory file path
    out << last_inventory_path << " \n"; // output inventory path

    // write current password
    out << GlobalVars::cur_password << " \n"; // output password

    file.close();
    return true;
}


void copyFolder(const QString &sourceFolder, const QString &destinationFolder)
{
    QDir sourceDir(sourceFolder);
    if (!sourceDir.exists())
        return;

    QDir destDir(destinationFolder);
    if (!destDir.exists()) {
        destDir.mkpath(destinationFolder);
    }

    QFileInfoList fileList = sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    foreach (QFileInfo fileInfo, fileList) {
        QString srcFilePath = fileInfo.absoluteFilePath();
        QString destFilePath = destinationFolder + "/" + fileInfo.fileName();

        if (fileInfo.isDir()) {
            copyFolder(srcFilePath, destFilePath);
        } else {
            QFile::copy(srcFilePath, destFilePath);
        }
    }
}
