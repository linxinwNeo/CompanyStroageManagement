#include <QMessageBox>
#include <QVector>
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include "ReadFile.h"
#include <QStandardPaths>
#include "GlobalVars.h"

ReadFile::ReadFile()
{
    inventory = Inventory();
}

ReadFile::~ReadFile()
{
}

/* read models file, while reading models, we also build container instances */
void ReadFile::read_Inventory_txt_File(const QString& path) const
{
    QFile file(path);
    QTextStream in(&file);
    qDebug() << "Start Reading" << path;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "couldn't open the file" << path;
        return;
    }

    QString line;
    { // read first two lines
        line = in.readLine(); // read the first line, which contains the number of models and the number of containers
        QStringList strList = line.split(split_item);
        unsigned int num_models = strList[0].toUInt() + 1;
        unsigned int num_containers = strList[1].toUInt() + 1;
        inventory.reserve_model_space(num_models * 1.5);
        inventory.reserve_container_space(num_containers * 1.3);

        line = in.readLine(); // read the second line, which contains the tags
    }

    while (!in.atEnd()) {
        line = in.readLine();

        QStringList strList = line.split(split_item);
        QSharedPointer<Model> m(new Model());
        m->MODEL_CODE = strList[0];
        if(m->MODEL_CODE.isEmpty()) {
            qDebug() << "encounter an empty model";
            continue;
        }
        m->DESCRIPTION_SPAN = strList[1];
        m->DESCRIPTION_CN = strList[2];
        m->PRIZE = strList[3].toDouble();
        m->NUM_INIT_BOXES = strList[4].toDouble();
        m->NUM_SOLD_BOXES = strList[5].toDouble();
        m->NUM_LEFT_ITEMS = strList[6].toInt();
        m->NUM_LEFT_BOXES = strList[7].toDouble();
        m->NUM_ITEMS_PER_BOX = strList[8].toInt();
        QString containerID = strList[9];

        // check if container exists
        if(containerID.startsWith("-1")){
            m->container = nullptr; // do nothing if this model does not have
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
            m->container = container;// add container to this model
        }
        inventory.add_Model(m);
    }

    file.close();
    qDebug() << "Reading" << path << "done, it has" << inventory.num_models() << "Models, "
             << inventory.num_containers() << "Containers";
}


// read inventory.xlsx file
void ReadFile::read_Inventory_xlsx_File(const QString &path) const
{
    //TODO
}


// read past lists from txt file
void ReadFile::read_Lists_txt_File(const QString &path) const
{
    lists.clear();

    QFile file(path);
    QTextStream in(&file);
    qDebug() << "Start Reading" << path;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "couldn't open the file" << path;
        return;
    }

    QString line;
    { // read first two lines
        line = in.readLine(); // read the first line, which contains the number of lists
        QStringList strList = line.split(split_item);
        unsigned int num_lists = strList[0].toUInt() + 1;
        if(num_lists == 0) return;
        lists.lists.reserve(num_lists);

        line = in.readLine(); // read the second line, which contains nothing
    }


    while (!in.atEnd()) {
        line = in.readLine();
        QStringList strList = line.split(split_item);

        ListPtr new_list (new List());

        // read id
        new_list->id = strList[0].toULong();

        // read date and time for the list
        new_list->date_created = QDate::fromString(strList[1], "dd MMM yyyy");
        new_list->time_created = QTime::fromString(strList[2], "hh:mm:ss");

        // read client_info
        new_list->client_info.CLIENTE = strList[3];
        new_list->client_info.DOMICILIO = strList[4];
        new_list->client_info.CIUDAD = strList[5];
        new_list->client_info.RFC = strList[6];
        new_list->client_info.AGENTE = strList[7];
        new_list->client_info.CONDICIONES = strList[8];
        new_list->client_info.TOTAL_NUM_BOXES = strList[9].toDouble();
        new_list->client_info.DISCOUNT = strList[10].toDouble();


        // reading entries
        unsigned long num_items = strList[11].toUInt();
        for(unsigned int i = 0; i < num_items; i++){
            line = in.readLine();
            QStringList entryRawData = line.split(split_item);

            EntryPtr newEntry(new Entry());

            newEntry->CLAVE = entryRawData[0];
            newEntry->ContainerID = entryRawData[1];
            newEntry->CAJA = entryRawData[2].toDouble();
            newEntry->CANTIDAD = entryRawData[3].toDouble();
            newEntry->CANT_POR_CAJA = entryRawData[4].toUInt();
            newEntry->Description_SPAN = entryRawData[5];
            newEntry->Description_CN = entryRawData[6];
            newEntry->PRECIO = entryRawData[7].toDouble();
            newEntry->IMPORTE = entryRawData[8].toDouble();

            new_list->add_item(newEntry);
        }

        lists.add_list(new_list);
    }

    file.close();
    qDebug() << "Reading" << path << "done, it has" << lists.num_lists() << "lists";
}
