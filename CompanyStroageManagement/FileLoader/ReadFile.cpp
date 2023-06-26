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
    this->container_idx_of_models.clear();
}

ReadFile::~ReadFile()
{
}

// read models file
void ReadFile::read_Models(const QString& path)
{
    QFile file(path);
    QTextStream in(&file);
    qDebug() << "reading" << path;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "couldn't open the file" << path;
        return;
    }

    QString line;
    line = in.readLine(); // read the second line

    while (!in.atEnd()) {
        line = in.readLine();

        QStringList strList = line.split("&&");
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
        if(containerID.startsWith("-1")){
            m->container = nullptr;
        }
        else{
            this->container_idx_of_models[m] = containerID.toInt();
        }

        inventory.add(m);
    }

    file.close();
    qDebug() << "Reading" << path << "done, it has" << inventory.models_size() << "Models";
}

// read containers file
void ReadFile::read_Containers(const QString &path)
{

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
