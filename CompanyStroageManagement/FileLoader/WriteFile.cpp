#include "WriteFile.h"
#include "GlobalVars.h"

WriteFile::WriteFile()
{

}

// write containers to a containers.txt file
void WriteFile::containers2txt(const QString &path)
{

}

// write models to a models.txt file
void WriteFile::models2txt(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "WriteFile::WriteToFile: couldn't create the file: " << path;
        return;
    }

    QTextStream out(&file);
    const QString split_item = "&&";
    for(const auto& m : inventory.model_vec){
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
            out << "-1\n";
        }
        else{
            out << m->container->ID + "\n";
        }
    }
    file.close();
    return;
}
