#ifndef READFILE_H
#define READFILE_H

#include "DataStructures/Model.h"
#include <QString>
#include <QFile>
#include <QSharedPointer>

class ReadFile {
private:
    QHash<ModelPtr, UI> container_idx_of_models;

public:
    static QString get_file_path();

    // member functions
    ReadFile();
    ~ReadFile();

    void read_Models(const QString& path);
    void read_Containers(const QString& path);
};

#endif //READFILE_H
