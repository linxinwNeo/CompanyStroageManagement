#include "ReadFile.h"

#include <QVector>
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QRegularExpression>

#include "GlobalVars.h"
#include "header/xlsxdocument.h"
#include "IO_Base.h"
#include "Others/write_error_file.h"

ReadFile::ReadFile()
{

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
        m->m_NUM_PIECES_PER_BOX = strList[6].toULong(); // 6.每箱个数/NUM_PIECES_PER_BOX
        m->m_PRICE = strList[7].toDouble(); // 7. 单价/PRIZE_PER_PIECE
        // 8. 上次修改时间/TIME_MODIFIED
        m->m_last_time_modified = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(strList[8], GlobalVars::DateTimeFormat)
            );

        // 9. 创建时间
        m->m_time_created = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(strList[9], GlobalVars::DateTimeFormat)
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
        m->m_NUM_PIECES_PER_BOX = xlsx.read(row, col++).toString().remove(',').toULong(); // 6. 每箱个数/NUM_PIECES_PER_BOX
        m->m_PRICE = xlsx.read(row, col++).toString().remove(',').toDouble(); // 7. 单价/PRIZE_PER_PIECE

        // 8. 上次修改时间/TIME_MODIFIED
        m->m_last_time_modified = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(xlsx.read(row, col++).toString(), GlobalVars::DateTimeFormat)
            );

        // 9. 创建时间
        m->m_time_created = QSharedPointer<QDateTime>::create(
            QDateTime::fromString(xlsx.read(row, col++).toString(), GlobalVars::DateTimeFormat)
            );

        inventory.add_new_Model(m);
    }

    qDebug() << "Read file" << path << "done, it has" <<
            inventory.num_models() << "models and" <<
            inventory.num_containers() << "containers.";

    if(save_path) last_inventory_path = path;

    return true;
}


// save the settings to the file
bool ReadFile::read_settings_file()
{
    QFile file(GlobalVars::Settings_FileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Couldn't open the file" << GlobalVars::Settings_FileName;

        return false;
    }

    QTextStream in(&file);


    // read language option
    QString line = in.readLine();
    GlobalVars::language_option = line.toUInt();
    if(GlobalVars::language_option > 1){
        GlobalVars::language_option = 0;
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


    // read password
    line = in.readLine().trimmed();
    GlobalVars::cur_password = line;

    file.close();
    return true;
}


bool ReadFile::Read_List(const QString& FilePath, ListPtr& list){
    QFile file(FilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        write_error_file("ReadFile::Read_List: couldn't read the file: " + FilePath);
        return false;
    }

    QTextStream in(&file);

    QStringList items = in.readLine().trimmed().split(split_item);
    // read list id
    list = ListPtr (new List());

    int idx = 0;

    list->id = items[idx++].toULong();
    list->datetime_created = QSharedPointer<QDateTime>::create(
        QDateTime::fromString(items[idx++], GlobalVars::DateTimeFormat)
        );

    list->client_info.m_ID = items[idx++];
    list->client_info.m_clientName = items[idx++];
    list->client_info.m_DOMICILIO = items[idx++];
    list->client_info.m_CIUDAD = items[idx++];
    list->client_info.m_RFC = items[idx++];
    list->client_info.m_CONDICIONES = items[idx++];
    list->client_info.m_TOTAL_NUM_BOXES = items[idx++].toDouble();
    list->client_info.m_DISCOUNT = items[idx++].toDouble();
    const unsigned long num_model_types = items[idx++].toDouble();

    list->entryList.entries.reserve(num_model_types);

    for(unsigned long i = 0; i < num_model_types; i++){
        EntryPtr entry(new Entry());
        QStringList entryItems = in.readLine().trimmed().split(split_item);

        idx = 0;
        entry->MODEL_CODE = entryItems[idx++];
        entry->ContainerID = entryItems[idx++];
        entry->NUM_PIECES = entryItems[idx++].toULong();
        entry->NUM_PIECES_PER_BOX = entryItems[idx++].toULong();
        entry->Description_SPAN = entryItems[idx++];
        entry->Description_CN = entryItems[idx++];
        entry->PRICE_PER_PIECE = entryItems[idx++].toDouble();
        entry->TOTAL = entryItems[idx++].toDouble();
        list->add_item(entry);
    }

    file.close();
    return true;
}


bool ReadFile::Read_Clients()
{
    QString path_to_clients_file = "./" + GlobalVars::Clients_FileName;

    QFile file(path_to_clients_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }

    QTextStream in(&file);
    unsigned long num_clients = in.readLine().trimmed().toULong();
    clientManager.reserve(num_clients + 1);

    for(unsigned long i = 0; i < num_clients; i++){
        QStringList clientInfo = in.readLine().trimmed().split(split_item);

        QSharedPointer<Client> client = QSharedPointer<Client>::create( Client(
            clientInfo[0], clientInfo[1], clientInfo[2], clientInfo[3],
            clientInfo[4], clientInfo[5]
            ) );

        clientManager.add_client(client);
    }

    return true;
}
