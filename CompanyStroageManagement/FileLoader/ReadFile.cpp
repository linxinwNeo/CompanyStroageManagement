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
void ReadFile::read_InventoryFile(const QString& path)
{
    QFile file(path);
    QTextStream in(&file);
    qDebug() << "Start Reading" << path;

    const QString split_item = "&&";

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
                inventory.add_container(container);
            }
            container->add_model(m); // add this model to the container
        }
        inventory.add_model(m);
    }

    file.close();
    qDebug() << "Reading" << path << "done, it has" << inventory.num_models() << "Models, "
             << inventory.num_containers() << "Containers";
}


QString ReadFile::get_file_path()
{
    QString filter = "Vector Field File (*.3dvf);; Polygonal File Format (*.ply)";
    QString filename = QFileDialog::getOpenFileName(nullptr, "Save File",
                                                    QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) +
                                                        "/untitled.3dvf",
                                                    filter);
    if(filename.isEmpty()){
        qDebug() << "failed to get filename, quit.";
        exit(1);
    }
    return filename;
}
