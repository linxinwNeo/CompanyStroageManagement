#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>

class WriteFile
{
public:
    WriteFile();
    ~WriteFile();

    void Models2txt(const QString& path) const;
    void Models2xlsx(const QString& path) const;

    void Lists2txt(const QString& path) const;
};

#endif // WRITEFILE_H
