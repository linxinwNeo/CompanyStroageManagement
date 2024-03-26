#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>


class WriteFile {
public:

    WriteFile();
    ~WriteFile() {};

    static bool SaveInventoryAuto(const bool save_path);
    static bool SaveInventoryAuto(const QString& path, const bool save_path);

    static bool Inventory2Txt(const QString& path, const bool save_path);
    static bool Inventory2Xlsx(const QString& path, const bool save_path);

    static bool Lists2txt(const bool save_path);
    static bool Lists2txt(const QString& path, const bool save_path);

    static bool update_BackUpDate();
    static bool save_BackUp_files(const bool save_path);

    static bool save_settings_file();
};

#endif // WRITEFILE_H
