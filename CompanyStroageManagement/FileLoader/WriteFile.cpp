#include "WriteFile.h"
#include "CN_Strings.h"
#include "GlobalVars.h"
#include "Others/output_error_file.h"
#include "SpanStrings.h"
#include "qdir.h"
#include "qmessagebox.h"
#include "header/xlsxdocument.h"

/* write models to a models.txt file
 * for each model, we need to output its properties, more specifically, output the container's ID */
bool WriteFile::Inventory2Txt(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::Models2txt: couldn't create the file: " + path);
        return false;
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
    return true;
}


/* write the models to a xlsx file */
bool WriteFile::Inventory2Xlsx(const QString &path) const
{
    using namespace QXlsx;

    // create a new file
    QXlsx::Document xlsx;

    unsigned int long row = 1;
    unsigned int long col = 1;

    // write the info about each column
    xlsx.write(row, col++, "货号");
    xlsx.write(row, col++, "品名（中文）");
    xlsx.write(row, col++, "品名（西语）");
    xlsx.write(row, col++, "单价");
    xlsx.write(row, col++, "进货箱数");
    xlsx.write(row, col++, "卖出箱数");
    xlsx.write(row, col++, "剩余箱数");
    xlsx.write(row, col++, "剩余件数");
    xlsx.write(row, col++, "每箱件数");
    xlsx.write(row, col++, "集装箱");

    row ++;

    QVector<ModelPtr> models;
    inventory.modelSet_2_Vector(models); // also sorts the models

    // write models to it
    for(const ModelPtr& model : models){
        col = 1;
        xlsx.write(row, col++, model->MODEL_CODE);
        xlsx.write(row, col++, model->DESCRIPTION_CN);
        xlsx.write(row, col++, model->DESCRIPTION_SPAN);
        xlsx.write(row, col++, model->PRIZE);
        xlsx.write(row, col++, model->NUM_INIT_BOXES);
        xlsx.write(row, col++, model->NUM_SOLD_BOXES);
        xlsx.write(row, col++, model->NUM_LEFT_BOXES);
        xlsx.write(row, col++, QString::number(model->NUM_LEFT_ITEMS));
        xlsx.write(row, col++, QString::number(model->NUM_ITEMS_PER_BOX));
        if(!model->container.isNull()) xlsx.write(row, col++, model->container->ID);
        row++;
    }

    // Save the file
    const bool success = xlsx.saveAs(path);
    if(!success){
        QMessageBox Msgbox(nullptr);
        if(language_option == 0) Msgbox.setText(SAVE_ERROR_MSG_CN);
        else if(language_option == 1) Msgbox.setText(SAVE_ERROR_MSG_SPAN);
        Msgbox.setStyleSheet("QLabel{min-width: 300px; min-height: 50px;}");
        Msgbox.exec();
        return false;
    }

    return true;
}


/* write lists to a txt file */
bool WriteFile::Lists2txt(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        write_error_file("WriteFile::Lists2txt: couldn't create the file: " + path);
        return false;
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

    return true;
}


// we update the BackUpDate
void WriteFile::update_BackUpDate() const
{
    QFile file(BackUP_FileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        QDateTime currentDateTime = QDateTime::currentDateTime();

        stream << currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

        // Close the file when done
        file.close();
    }
    else{
        // Display an error message box
        QMessageBox::critical(nullptr, "Error", UNABLE_TO_SAVE_BACKUP_DATE_MSG);
    }
}


// save the back up files in a folder
bool WriteFile::save_BackUp_files() const
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString folderName = currentDateTime.toString("yyyy-MM-dd-HH-mm-ss");
    QDir timeDir(BackUP_DirName + "/" + folderName);
    // Create the subfolder named by datetime
    if (!timeDir.exists()) {
        timeDir.mkpath(".");
    }

    QString path_to_list_file = BackUP_DirName + "/" + folderName + "/lists.txt";
    if(!this->Lists2txt(path_to_list_file)) return false;
    QString path_to_inventory_file = BackUP_DirName + "/" + folderName + "/inventory.xlsx";
    if(!this->Inventory2Xlsx(path_to_inventory_file)) return false;

    return true;
}
