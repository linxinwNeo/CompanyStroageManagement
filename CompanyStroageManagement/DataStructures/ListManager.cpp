#include "ListManager.h"
#include "Others/write_error_file.h"
#include "FileLoader/WriteFile.h"
#include "FileLoader/ReadFile.h"
#include "GlobalVars.h"
#include "Algorithm/QuickSort.h"

#include <QDir>
#include <QRegularExpression>

static QRegularExpression re("(\\d+)");

/**************************ListManager*************************/
ListManager::ListManager() {
    this->clear();
};


ListManager::~ListManager() {
    this->clear();
};


unsigned long int ListManager::get_unique_id() const
{
    return this->max_id + 1; // add one more to it
}


// iterate every list file name and find the one with largest id
void ListManager::update_max_id()
{
    static QRegularExpressionMatch match;

    QString folderPath = "./" + GlobalVars::Lists_DirName;
    QDir directory(folderPath);
    // Get the list of all files in the directory
    QStringList files = directory.entryList(QDir::Files);

    unsigned long max = 0;
    // Iterate and print the file names
    foreach (QString fileName, files) {
        match = re.match(fileName);

        if (match.hasMatch()) {
            unsigned long number = match.captured(1).toULong();
            if(number > max) max = number;
        }
    }

    this->max_id = max;
}


void ListManager::create_list(ListPtr list_2be_added)
{
    if(list_2be_added.isNull()){
        write_error_file("ListManager::create_list: trying to add a null list");
        return;
    }

    if(!WriteFile::Save_List(list_2be_added)){
        write_error_file("ListManager::create_list: fail to save a list");
        return;
    }

    this->update_max_id();
}


// remove the list with id and return the corresponding list, null if not exists
void ListManager::delete_list(const unsigned long id)
{
    // create the path
    const QString filePath = "./" + GlobalVars::Lists_DirName + "/" +
                       QString::number(id) + ".txt";

    QFile file(filePath);
    // check if the file exists
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        // not exists, we do nothing
    }
    else{
        // file exists, we delete it
        if(!file.remove()){
            write_error_file("Fails to delete the list with id: " + QString::number(id));
        }
    }

    return;
}


// get the reference to the list with specified id
ListPtr ListManager::get_list(const unsigned long id)
{
    ListPtr list = nullptr;
    ReadFile::Read_List(id, list);

    return list;
}


// get the lists with their id begin with id_prefix
// if prefix is empty, we return all lists
void ListManager::get_lists_by_listID_prefix(const QString id_prefix, QVector<ListPtr>& candidates, bool sorted)
{
    const QString new_str = id_prefix.toUpper().trimmed();

    // if id_prefix is empty we return nothing
    if(new_str.isEmpty()){
        candidates.clear();
        return;
    }

    static QRegularExpressionMatch match;

    QString folderPath = "./" + GlobalVars::Lists_DirName;
    QDir directory(folderPath);
    // Get the list of all files in the directory
    QStringList files = directory.entryList(QDir::Files);

    QVector<QString> exist_ids;
    exist_ids.reserve(files.size() + 1);

    candidates.clear();
    candidates.reserve(files.size() + 1);

    // Iterate and print the file names
    foreach (QString fileName, files) {
        match = re.match(fileName);

        if (match.hasMatch()) {
            QString id = match.captured(1);
            exist_ids.push_back(id);
        }
    }


    // for each list , we convert it to a string and testing
    for(QString& id : exist_ids){
        if(id.startsWith(new_str)){
            ListPtr list = nullptr;

            ReadFile::Read_List(id.toULong(), list);

            if(!list.isNull()) candidates.push_back(list);
        }
    }

    // sort the vector by list ids if needed
    if(sorted){
        QuickSorts::QuickSort(candidates);
    }

    return;
}

void ListManager::get_lists_by_clientID_prefix(const QString id_prefix, QVector<QSharedPointer<List> > &candidates, bool sorted)
{
    const QString new_str = id_prefix.toUpper().trimmed();

    // if id_prefix is empty we return nothing
    if(new_str.isEmpty()){
        candidates.clear();
        return;
    }

    static QRegularExpressionMatch match;

    QString folderPath = "./" + GlobalVars::Lists_DirName;
    QDir directory(folderPath);
    // Get the list of all files in the directory
    QStringList files = directory.entryList(QDir::Files);

    QVector<QString> exist_ids;
    exist_ids.reserve(files.size() + 1);

    candidates.clear();
    candidates.reserve(files.size() + 1);

    // Iterate and print the file names
    foreach (QString fileName, files) {
        match = re.match(fileName);

        if (match.hasMatch()) {
            QString id = match.captured(1);
            exist_ids.push_back(id);
        }
    }


    // for each list , we convert it to a string and testing
    for(QString& id : exist_ids){
        ListPtr list = nullptr;

        ReadFile::Read_List(id.toULong(), list);

        if(!list.isNull() && list->client_info.m_ID.toUpper().startsWith(new_str))
            candidates.push_back(list);
    }

    // sort the vector by list ids if needed
    if(sorted){
        QuickSorts::QuickSort(candidates);
    }

    return;
}

void ListManager::get_lists_by_clientName_prefix(const QString name_prefix, QVector<QSharedPointer<List> > &candidates, bool sorted)
{
    const QString new_str = name_prefix.toUpper().trimmed();

    // if id_prefix is empty we return nothing
    if(new_str.isEmpty()){
        candidates.clear();
        return;
    }

    static QRegularExpressionMatch match;

    QString folderPath = "./" + GlobalVars::Lists_DirName;
    QDir directory(folderPath);
    // Get the list of all files in the directory
    QStringList files = directory.entryList(QDir::Files);

    QVector<QString> exist_ids;
    exist_ids.reserve(files.size() + 1);

    candidates.clear();
    candidates.reserve(files.size() + 1);

    // Iterate and print the file names
    foreach (QString fileName, files) {
        match = re.match(fileName);

        if (match.hasMatch()) {
            QString id = match.captured(1);
            exist_ids.push_back(id);
        }
    }


    // for each list , we convert it to a string and testing
    for(QString& id : exist_ids){
        ListPtr list = nullptr;

        ReadFile::Read_List(id.toULong(), list);

        if(!list.isNull() && list->client_info.m_clientName.toUpper().startsWith(new_str))
            candidates.push_back(list);
    }

    // sort the vector by list ids if needed
    if(sorted){
        QuickSorts::QuickSort(candidates);
    }

    return;
}


void ListManager::clear()
{
    this->max_id = 0;
}

