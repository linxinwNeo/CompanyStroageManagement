#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>
#include "QXlsx/QXlsxHeaders.h"

class WriteFile
{
public:
    const QString split_item = "&&";

    WriteFile() {};
    ~WriteFile() {};

    void Inventory2Txt(const QString& path) const;
    void Inventory2Xlsx(const QString& path) const;

    void Lists2txt(const QString& path) const;

    void update_BackUpDate() const;
    void save_BackUp_files() const;
};

#endif // WRITEFILE_H
