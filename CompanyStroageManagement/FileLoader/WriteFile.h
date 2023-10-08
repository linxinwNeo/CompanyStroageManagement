#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>

class WriteFile
{
public:
    const QString split_item = "&&";

    WriteFile() {};
    ~WriteFile() {};

    bool Inventory2Txt(const QString& path) const;
    bool Inventory2Xlsx(const QString& path) const;

    bool Lists2txt(const QString& path) const;

    void update_BackUpDate() const;
    bool save_BackUp_files() const;

    bool save_settings_file() const;
};

#endif // WRITEFILE_H
