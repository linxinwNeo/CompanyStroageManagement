#ifndef READFILE_H
#define READFILE_H

#include "DataStructures/Model.h"
#include <QString>
#include <QFile>
#include <QSharedPointer>

class ReadFile {

public:
    static QString get_file_path();

    // member functions
    ReadFile();
    ~ReadFile();

    void read_Inventory_txt_File(const QString& path) const;
    void read_Inventory_xlsx_File(const QString& path) const;

    void read_Lists_txt_File(const QString& path) const;
};

#endif //READFILE_H
