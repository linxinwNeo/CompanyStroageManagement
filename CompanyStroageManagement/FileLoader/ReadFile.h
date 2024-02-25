#ifndef READFILE_H
#define READFILE_H

#include "DataStructures/Model.h"
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

    static bool read_Lists_txt_File(const bool save_path);
    static bool read_Lists_txt_File(const QString& path, const bool save_path);
    static bool read_settings_file();
};

#endif //READFILE_H
