#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>

class WriteFile
{
public:
    WriteFile();
    ~WriteFile();

    void models2txt(const QString& path);
    void models2xlsx(const QString& path);
};

#endif // WRITEFILE_H
