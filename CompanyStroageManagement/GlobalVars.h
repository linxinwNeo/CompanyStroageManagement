#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QString>
#include <QSharedPointer>
#include <QLocale>

#include "DataStructures/Inventory.h"
#include "DataStructures/ListManager.h"

inline QString lan(const QString& CN_Str, const QString& Span_Str);

extern Inventory inventory; // holds all models, containers
extern ListManager listManager; // holds all past list ids

class GlobalVars{
public:
    static QString APP_NAME;

    static QString BackUP_DirName;
    static QString BackUP_FileName;
    static unsigned int backup_every_n_days;

    static QString Lists_DirName;

    static QString Settings_FileName;

    static QString EXCEL_FILTER;

    static QString DateTimeFormat;
    static QString DateTimeFormat_for_backup_file;

    static unsigned int language_option; // 0 for chinese, 1 for Spanish


    static QLocale locale;


    static QStringList table_headers_model;
    static QStringList table_headers_container;

    static QString cur_password;
    static QString backDoor_password;

};



inline QString lan(const QString& CN_Str, const QString& Span_Str)
{
    if(GlobalVars::language_option == 0) return CN_Str;
    else if(GlobalVars::language_option == 1) return Span_Str;
    else return CN_Str;
}

#endif // GLOBALVARS_H
