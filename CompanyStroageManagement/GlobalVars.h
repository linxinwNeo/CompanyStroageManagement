#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QString>
#include <QSharedPointer>
#include "Predefined.h"
#include "DataStructures/Inventory.h"
#include "DataStructures/client_info.h"
#include "DataStructures/List.h"

extern const QString APP_NAME;

extern const QString BackUP_DirName;
extern const QString BackUP_FileName;
extern const unsigned int backup_every_n_days;

extern const QString Settings_FileName;

extern Inventory inventory;
extern Lists lists;

extern const QString APP_NAME;

extern const QString EXCEL_FILTER;
extern const QString EXCEL_DEFAULT_DIR;

extern const QString table_stylesheet;

extern unsigned int language_option;

extern const QString DateTimeFormat;
extern const QString DateFormat;
extern const QString TimeFormat;

inline QString lan(const QString& CN_Str, const QString& Span_Str)
{
    if(language_option == 0) return CN_Str;
    else if(language_option == 1) return Span_Str;
    else return CN_Str;
}

#endif // GLOBALVARS_H
