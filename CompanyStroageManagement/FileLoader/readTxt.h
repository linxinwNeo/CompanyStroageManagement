#ifndef READTXT_H
#define READTXT_H

#include "DataStructures/model.h"
#include <QString>
#include <QFile>
#include <QSharedPointer>

class ReadTxt {
public:
    static QString get_file_path();

    // member functions
    ReadTxt();
    ~ReadTxt();

    void read_Models(const QString& path);
    void read_Containers(const QString& path);
};

#endif //READTXT_H
