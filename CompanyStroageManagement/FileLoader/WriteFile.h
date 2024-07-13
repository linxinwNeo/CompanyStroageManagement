#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>
#include "DataStructures/List.h"

class WriteFile {
public:
    WriteFile();
    ~WriteFile() {};

    static bool SaveInventoryAuto(const bool save_path);
    static bool SaveInventoryAuto(const QString& path, const bool save_path);

    static bool Inventory2Txt(const QString& path, const bool save_path);
    static bool Inventory2Xlsx(const QString& path, const bool save_path);

    static bool update_BackUpDate();
    static bool save_BackUp_files(const bool save_path);

    static bool save_settings_file();

    static bool Save_List(const ListPtr list);

    static bool Save_Clients();
};

void copyFolder(const QString &sourceFolder, const QString &destinationFolder);

#endif // WRITEFILE_H
