#include "ReadFile.h"

#include <QVector>
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>


#include "GlobalVars.h"
#include "header/xlsxdocument.h"
#include "IO_Base.h"
#include "Others/write_error_file.h"

ReadFile::ReadFile()
{
    inventory = Inventory();
}

ReadFile::~ReadFile()
{
}


// based on saved path, automatically select a function to read
bool ReadFile::read_Inventory_File_Auto(const bool save_path)
{
    if(last_inventory_path.endsWith(".xlsx")){
        return read_Inventory_xlsx_File(last_inventory_path, save_path);
    }
    else if(last_inventory_path.endsWith(".txt")){
        return read_Inventory_txt_File(last_inventory_path, save_path);
    }
    else{
        return false;
    }
}


bool ReadFile::read_Inventory_File_Auto(const QString &path, const bool save_path)
{
    if(path.endsWith(".xlsx")){
        return read_Inventory_xlsx_File(path, save_path);
    }
    else if(path.endsWith(".txt")){
        return read_Inventory_txt_File(path, save_path);
    }
    else{
        return false;
    }
}


/* read models file, while reading models, we also build m_Container instances */
bool ReadFile::read_Inventory_txt_File(const QString& path, const bool save_path)
{
    QFile file(path);
    QTextStream in(&file);
    qDebug() << "Start Reading" << path;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }

    QString line;
    { // read first two lines
        line = in.readLine(); // read the first line, which contains the number of models and the number of containers
        QStringList strList = line.split(split_item);
        unsigned int num_models = strList[0].toULong() + 1;
        unsigned int num_containers = strList[1].toULong() + 1;

        inventory.clear();
        inventory.reserve_model_space(num_models * 1.5);
        inventory.reserve_container_space(num_containers * 1.3);

        line = in.readLine(); // read the second line, which contains the tags
    }

    while (!in.atEnd()) {
        line = in.readLine();

        QStringList strList = line.split(split_item);
        QSharedPointer<Model> m(new Model());
        m->m_MODEL_CODE = strList[0]; // 0. 货号/MODEL_CODE
        if(m->m_MODEL_CODE.isEmpty()) {
            write_error_file("Encounter an empty Model CODE");
            continue;
        }

        QString containerID = strList[1].trimmed(); // 1. 集装箱号/CONTAINER_ID
        // check if the container exists
        if(containerID.startsWith("-1")){
            m->m_Container = nullptr; // do nothing if this model does not have a container
        }
        else{
            /* this model has a container, we need to check if this container has been created already
             * we create a new container instance if it has not been created. */
            ContainerPtr container (nullptr);
            if(inventory.contains_container(containerID)){
                // this container already exists, we get reference from inventory
                container = inventory.get_container(containerID);

                //  we should never reach here
                if(!container){
                    write_error_file("Fails to retrice container instance. It may be caused by incorrect continer id number.");
                }
            }
            else{
                // this container does not exist, we need to create a new one and add it to inventory
                container = ContainerPtr (new Container(containerID));
                inventory.add_Container(container);
            }
            container->add_model(m); // add this model to the container
            m->m_Container = container;// add container to this model
        }

        m->m_DESCRIPTION_CN = strList[2]; // 2. 品名（中文）/DESCRIPTION_CN
        m->m_DESCRIPTION_SPAN = strList[3]; // 3. 品名（西语）/DESCRIPTION_SPAN

        m->m_NUM_INIT_PIECES = strList[4].toULong(); // 4. 进货个数/NUM_INITIAL_PIECES
        m->m_NUM_SOLD_PIECES = strList[5].toULong(); // 5. 已售个数/NUM_SOLD_PIECES
        m->m_NUM_PIECES_PER_BOX = strList[7].toULong(); // 6.每箱个数/NUM_PIECES_PER_BOX
        m->m_PRIZE = strList[8].toDouble(); // 7. 单价/PRIZE_PER_PIECE
        // 8. 上次修改时间/TIME_MODIFIED
        m->m_last_time_modified = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(strList[9], DateTimeFormat)
            );

        // 9. 创建时间
        m->m_time_created = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(strList[10], DateTimeFormat)
            );

        inventory.add_new_Model(m);
    }

    file.close();
    qDebug() << "Reading" << path << "done, it has" << inventory.num_models() << "Models, "
             << inventory.num_containers() << "Containers";

    if(save_path) last_inventory_path = path;

    return true;
}


// read inventory.xlsx file
bool ReadFile::read_Inventory_xlsx_File(const QString &path, const bool save_path)
{
    QXlsx::Document xlsx(path);

    unsigned long row = 2;
    unsigned long col = 1;
    unsigned long rowCount = 0;

    bool selected_a_sheet = xlsx.selectSheet(0);

    // check if the file can be opened and counting the number of rows
    if (selected_a_sheet) {
        // Iterate through the rows until you encounter an empty row
        while (!xlsx.read(rowCount + 1, 1).isNull()) {
            rowCount++;
        }
    }
    else {
        return false;
    }

    // read file
    for(row = 2; row < rowCount + 2; row++){
        col = 1;
        QString modelCode = xlsx.read(row, col++).toString().trimmed(); // 0. 货号/MODEL_CODE
        if(modelCode.isEmpty()) continue; // skip empty rows

        QSharedPointer<Model> m(new Model());
        m->m_MODEL_CODE = modelCode;

        QString containerID = xlsx.read(row, col++).toString().trimmed(); // 1. 集装箱号/CONTAINER_ID
        // check if this container with this ID exists
        if(containerID.isEmpty() || containerID == "-1"){
            m->m_Container = nullptr; // do nothing if this model does not have a container
        }
        else{
            /* this model has a container, we need to check if this container has been created already
             * we create a new container instance if it has not been created. */
            ContainerPtr container (nullptr);
            if(inventory.contains_container(containerID)){
                // this container already exists, we get reference from inventory
                container = inventory.get_container(containerID);
            }
            else{
                // this container does not exist, we need to create a new one and add it to inventory
                container = ContainerPtr (new Container(containerID));
                inventory.add_Container(container);
            }
            container->add_model(m); // add this model to the container
            m->m_Container = container;// add container to this model
        }

        m->m_DESCRIPTION_CN = xlsx.read(row, col++).toString().trimmed(); // 2. 品名（中文）/DESCRIPTION_CN
        m->m_DESCRIPTION_SPAN = xlsx.read(row, col++).toString().trimmed(); // 3. 品名（西语）/DESCRIPTION_SPAN

        /* because when we output data, we used unsigned long long
         * we also need to handle the commas in the string */
        m->m_NUM_INIT_PIECES = xlsx.read(row, col++).toString().remove(',').toULong(); // 4. 进货个数/NUM_INITIAL_PIECES
        m->m_NUM_SOLD_PIECES = xlsx.read(row, col++).toString().remove(',').toULong(); // 5. 已售个数/NUM_SOLD_PIECES
        col++;
        m->m_NUM_PIECES_PER_BOX = xlsx.read(row, col++).toString().remove(',').toULong(); // 6. 每箱个数/NUM_PIECES_PER_BOX
        m->m_PRIZE = xlsx.read(row, col++).toString().remove(',').toDouble(); // 7. 单价/PRIZE_PER_PIECE

        // 8. 上次修改时间/TIME_MODIFIED
        m->m_last_time_modified = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(xlsx.read(row, col++).toString(), DateTimeFormat)
            );

        // 9. 创建时间
        m->m_time_created = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(xlsx.read(row, col++).toString(), DateTimeFormat)
            );

        inventory.add_new_Model(m);
    }

    qDebug() << "Read file" << path << "done, it has" <<
            inventory.num_models() << "models and" <<
            inventory.num_containers() << "containers.";

    if(save_path) last_inventory_path = path;

    return true;
}


bool ReadFile::read_Lists_txt_File(const bool save_path)
{
    return read_Lists_txt_File(last_lists_path, save_path);
}


// read past lists from txt file
bool ReadFile::read_Lists_txt_File(const QString &path, const bool save_path)
{
    lists.clear();

    QFile file(path);
    QTextStream in(&file);
    qDebug() << "Start Reading" << path;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }

    QString line;
    { // read first two lines
        line = in.readLine(); // read the first line, which contains the number of lists
        QStringList strList = line.split(split_item);
        unsigned int num_lists = strList[0].toUInt() + 1;
        if(num_lists == 0) return false;
        lists.lists.reserve(num_lists);

        line = in.readLine(); // read the second line, which contains nothing
    }


    while (!in.atEnd()) {
        line = in.readLine();
        QStringList strList = line.split(split_item);

        ListPtr new_list (new List());

        new_list->id = strList[0].toULong(); // 1. 清单号

        // read date and time for the list
        new_list->datetime_created = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(strList[1], DateTimeFormat)
            );      // 2. 读取创建时间

        // read client_info
        new_list->client_info.CLIENTE = strList[2]; // 2.
        new_list->client_info.DOMICILIO = strList[3]; // 3.
        new_list->client_info.CIUDAD = strList[4]; // 4.
        new_list->client_info.RFC = strList[5]; // 5.
        new_list->client_info.AGENTE = strList[6]; // 6.
        new_list->client_info.CONDICIONES = strList[7]; // 7.
        new_list->client_info.TOTAL_NUM_BOXES = strList[8].toDouble(); // 8.
        new_list->client_info.DISCOUNT = strList[9].toDouble(); // 9.

        // reading entries
        unsigned long num_items = strList[10].toUInt(); // 10.
        for(unsigned long i = 0; i < num_items; i++){
            line = in.readLine();
            QStringList entryRawData = line.split(split_item);

            EntryPtr newEntry(new Entry());

            newEntry->MODEL_CODE = entryRawData[0];
            newEntry->ContainerID = entryRawData[1];
            newEntry->NUM_PIECES = entryRawData[2].toULong();
            newEntry->NUM_PIECES_PER_BOX = entryRawData[3].toULong();
            newEntry->Description_SPAN = entryRawData[4];
            newEntry->Description_CN = entryRawData[5];
            newEntry->PRICE_PER_PIECE = entryRawData[6].toDouble();
            newEntry->TOTAL = entryRawData[7].toDouble();

            new_list->add_item(newEntry);
        }

        lists.add_list(new_list);
    }

    file.close();
    qDebug() << "Reading" << path << "done, it has" << lists.num_lists() << "lists";

    if(save_path)
        last_lists_path = path;

    return true;
}


// save the settings to the file
bool ReadFile::read_settings_file()
{
    QFile file(Settings_FileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Couldn't open the file" << Settings_FileName;

        return false;
    }

    QTextStream in(&file);


    // read language option
    QString line = in.readLine();
    language_option = line.toUInt();
    if(language_option > 1){
        language_option = 0;
    }

    if( in.atEnd() ){
        file.close();
        return false;
    }

    // read last xlsx path
    line = in.readLine().trimmed();
    if(!line.isEmpty() && (line.endsWith(".xlsx") || line.endsWith(".txt"))){
        // save the path if it is not empty
        last_inventory_path = line;
    }

    if( in.atEnd() ){
        file.close();
        return false;
    }

    // read last lists path
    line = in.readLine().trimmed();
    if(!line.isEmpty() && line.endsWith(".txt")){
        // save the path if it is not empty
        last_lists_path = line;
    }

    file.close();
    return true;
}
