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
    unsigned long max = 0;

    QDateTime DT = QDateTime::currentDateTime();

    const QString folderPath = "./" + GlobalVars::Lists_DirName + "/" +
                               DT.toString(GlobalVars::DateTimeFormat_year);
    QDir dir(folderPath);

    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot); // opens the current year folder

    // Iterate and print the file names
    foreach (QFileInfo fileInfo, fileInfoList) {
        QString subFolderPath = fileInfo.absoluteFilePath();

        QDir subDir(subFolderPath);
        QFileInfoList subFileInfoList = subDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot); // months
        foreach (QFileInfo subFileInfo, subFileInfoList) {
            const QString absPath = subFileInfo.absoluteFilePath();

            ListPtr list = nullptr;

            bool success = ReadFile::Read_List(absPath, list);

            if(success && !list.isNull())
            {
                if(list->id > max){
                    max = list->id;
                }
            }

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
bool ListManager::delete_list(const QDateTime& creationDateTime)
{
    // we are looking for the list file with <creationDateTime>.txt
    const QString filePath = "./" + GlobalVars::Lists_DirName + "/" +
                             creationDateTime.toString(GlobalVars::DateTimeFormat_year) + "/" +
                             creationDateTime.toString(GlobalVars::DateTimeFormat_month) + "/" +
                             creationDateTime.toString(GlobalVars::DateTimeFormat_for_file) + ".txt";

    QFile file(filePath);
    // check if the file exists
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        goto Fail;
    }
    else{
        // file exists, we try to delete it
        if(file.remove()){
            goto Success;
        }
        else{
            goto Fail;
        }
    }

Success:
    file.close();
    return true;

Fail:
    file.close();
    return false;
}


// get the reference to the list with specified id
ListPtr ListManager::get_list(const QDateTime& creationDateTime)
{
    // we are looking for the list file with <creationDateTime>.txt
    const QString filePath = "./" + GlobalVars::Lists_DirName + "/" +
                             creationDateTime.toString(GlobalVars::DateTimeFormat_year) + "/" +
                             creationDateTime.toString(GlobalVars::DateTimeFormat_month) + "/" +
                             creationDateTime.toString(GlobalVars::DateTimeFormat_for_file) + ".txt";


    ListPtr list = nullptr;

    ReadFile::Read_List(filePath, list);

    return list;
}


// get the lists with their id begin with id_prefix
// if prefix is empty, we return all lists
void ListManager::get_lists_by_listID_prefix(const QString id_prefix, QVector<ListPtr>& candidates, bool sorted)
{
    const QString ID_PREFIX = id_prefix.toUpper().trimmed();

    // if ID_PREFIX is empty we return nothing
    if(ID_PREFIX.isEmpty()){
        candidates.clear();
        return;
    }

    candidates.clear();
    candidates.reserve(500);

    const QString folderPath = "./" + GlobalVars::Lists_DirName;
    QDir YearsDir(folderPath);

    QFileInfoList yearFoldersInfoList = YearsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot); // years folders list

    foreach (QFileInfo yearFolderInfo, yearFoldersInfoList) {
        QString yearFolderPath = yearFolderInfo.absoluteFilePath(); // e.g., ./ListsRecords/2024

        QDir monthsDir(yearFolderPath);
        QFileInfoList monthsFolderInfoList = monthsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot); // months
        foreach (QFileInfo monthsFolderInfo, monthsFolderInfoList) {
            const QString absMonthFolderPath = monthsFolderInfo.absoluteFilePath(); // e.g., ./ListsRecords/2024/07

            QDir ListsDir(absMonthFolderPath);
            QFileInfoList listFilesInfoList = ListsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot); // list files

            foreach (QFileInfo listFileInfo, listFilesInfoList) {
                ListPtr list = nullptr;

                bool success = ReadFile::Read_List(listFileInfo.absoluteFilePath(), list);

                if(success && !list.isNull() &&
                    QString::number(list->id).startsWith(ID_PREFIX)
                )
                {
                    candidates.push_back(list);
                }
            }
        }
    }

    // sort the vector by list ids if needed
    if(sorted){
        QuickSorts::QuickSort(candidates);
    }
}

void ListManager::get_lists_by_clientID_prefix(const QString id_prefix, QVector<QSharedPointer<List> > &candidates, bool sorted)
{
    const QString ID_PREFIX = id_prefix.toUpper().trimmed();

    // if ID_PREFIX is empty we return nothing
    if(ID_PREFIX.isEmpty()){
        candidates.clear();
        return;
    }

    candidates.clear();
    candidates.reserve(500);

    const QString folderPath = "./" + GlobalVars::Lists_DirName;
    QDir YearsDir(folderPath);

    QFileInfoList yearFoldersInfoList = YearsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot); // years folders list

    foreach (QFileInfo yearFolderInfo, yearFoldersInfoList) {
        QString yearFolderPath = yearFolderInfo.absoluteFilePath(); // e.g., ./ListsRecords/2024

        QDir monthsDir(yearFolderPath);
        QFileInfoList monthsFolderInfoList = monthsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot); // months
        foreach (QFileInfo monthsFolderInfo, monthsFolderInfoList) {
            const QString absMonthFolderPath = monthsFolderInfo.absoluteFilePath(); // e.g., ./ListsRecords/2024/07

            QDir ListsDir(absMonthFolderPath);
            QFileInfoList listFilesInfoList = ListsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot); // list files

            foreach (QFileInfo listFileInfo, listFilesInfoList) {
                ListPtr list = nullptr;

                bool success = ReadFile::Read_List(listFileInfo.absoluteFilePath(), list);

                if(success && !list.isNull() && list->client_info.m_ID.toUpper().startsWith(ID_PREFIX)
                )
                {
                    candidates.push_back(list);
                }
            }
        }
    }

    // sort the vector by list ids if needed
    if(sorted){
        QuickSorts::QuickSort(candidates);
    }
}

void ListManager::get_lists_by_clientName_prefix(const QString name_prefix, QVector<QSharedPointer<List> > &candidates, bool sorted)
{
    const QString NAME_PREFIX = name_prefix.toUpper().trimmed();

    // if ID_PREFIX is empty we return nothing
    if(NAME_PREFIX.isEmpty()){
        candidates.clear();
        return;
    }

    candidates.clear();
    candidates.reserve(500);

    const QString folderPath = "./" + GlobalVars::Lists_DirName;
    QDir YearsDir(folderPath);

    QFileInfoList yearFoldersInfoList = YearsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot); // years folders list

    foreach (QFileInfo yearFolderInfo, yearFoldersInfoList) {
        QString yearFolderPath = yearFolderInfo.absoluteFilePath(); // e.g., ./ListsRecords/2024

        QDir monthsDir(yearFolderPath);
        QFileInfoList monthsFolderInfoList = monthsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot); // months
        foreach (QFileInfo monthsFolderInfo, monthsFolderInfoList) {
            const QString absMonthFolderPath = monthsFolderInfo.absoluteFilePath(); // e.g., ./ListsRecords/2024/07

            QDir ListsDir(absMonthFolderPath);
            QFileInfoList listFilesInfoList = ListsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot); // list files

            foreach (QFileInfo listFileInfo, listFilesInfoList) {
                ListPtr list = nullptr;

                bool success = ReadFile::Read_List(listFileInfo.absoluteFilePath(), list);

                if(success && !list.isNull() && list->client_info.m_clientName.toUpper().startsWith(NAME_PREFIX)
                    )
                {
                    candidates.push_back(list);
                }
            }
        }
    }

    // sort the vector by list ids if needed
    if(sorted){
        QuickSorts::QuickSort(candidates);
    }
}


void ListManager::clear()
{
    this->max_id = 0;
}

