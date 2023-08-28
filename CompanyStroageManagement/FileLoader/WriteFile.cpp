#include "WriteFile.h"
#include "GlobalVars.h"
#include "output_error_file.cpp"

WriteFile::WriteFile()
{

}

WriteFile::~WriteFile()
{

}


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
    const QString split_item = "&&";
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
    const QString split_item = "&&";

    // output total number of lists
    out << "% num of lists" << QString::number(lists.num_lists()) << "\n";
    out <<

    for(const ListPtr& list : lists.lists){
        out << QString::number(list->id)
    }
}
