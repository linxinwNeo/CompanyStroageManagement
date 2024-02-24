#ifndef READFILE_H
#define READFILE_H

#include "DataStructures/Model.h"
#include <QString>
#include <QFile>
#include <QSharedPointer>
#include "IO_Base.h"


class ReadFile {

public:
    // member functions
    ReadFile();
    ~ReadFile();

    static void read_Inventory_txt_File(const QString& path);

    static void read_Inventory_xlsx_File();
    static void read_Inventory_xlsx_File(const QString& path);

    static void read_Lists_txt_File(const QString& path);
    static bool read_settings_file();
};

#endif //READFILE_H
