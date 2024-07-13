#ifndef READFILE_H
#define READFILE_H

#include "DataStructures/Model.h"
#include "DataStructures/List.h"
#include <QString>
#include <QFile>
#include <QSharedPointer>


class ReadFile {

public:
    // member functions
    ReadFile();
    ~ReadFile();

    static bool read_Inventory_File_Auto(const bool save_path);
    static bool read_Inventory_File_Auto(const QString& path, const bool save_path);

    static bool read_Inventory_txt_File(const QString& path, const bool save_path);
    static bool read_Inventory_xlsx_File(const QString& path, const bool save_path);

    static bool read_settings_file();

    static bool Read_List(const unsigned long & id, ListPtr& list);

    static bool Read_Clients();
};

#endif //READFILE_H
