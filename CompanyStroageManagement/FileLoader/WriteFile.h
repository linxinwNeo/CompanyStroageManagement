#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QString>
#include "IO_Base.h"

class WriteFile {
public:

    WriteFile();
    ~WriteFile() {};

    static bool Inventory2Txt(const QString& path);

    static bool Inventory2Xlsx();
    static bool Inventory2Xlsx(const QString& path);

    static bool Lists2txt(const QString& path);

    void update_BackUpDate();
    bool save_BackUp_files();

    bool save_settings_file();
};

#endif // WRITEFILE_H
