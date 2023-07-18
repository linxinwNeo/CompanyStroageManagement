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

    void read_InventoryFile(const QString& path);
};

#endif //READFILE_H
