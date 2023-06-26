#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>

class WriteFile
{
public:
    WriteFile();
    ~WriteFile();

    void containers2txt(const QString& path);
    void models2txt(const QString& path);
};

#endif // WRITEFILE_H
