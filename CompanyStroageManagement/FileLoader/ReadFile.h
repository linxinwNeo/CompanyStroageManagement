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

    static bool Read_list(const unsigned long & id, ListPtr& list);
};

#endif //READFILE_H
