#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QString>
#include <QSharedPointer>
#include "Predefined.h"
#include "DataStructures/Inventory.h"
#include "DataStructures/client_info.h"
#include "DataStructures/List.h"

extern const QString APP_NAME;
extern QString Inventory_FNAME_xlsx;
extern QString Lists_FNAME;

extern const QString BackUP_DirName;
extern const QString BackUP_FileName;

extern Inventory inventory;
extern Lists lists;

extern const QString UNABLE_TO_SAVE_BACKUP_DATE_MSG;

extern const QString APP_NAME;

extern const QString EXCEL_FILTER;
extern const QString EXCEL_DEFAULT_DIR;

extern const QString table_stylesheet;

extern unsigned int language_option;

#endif // GLOBALVARS_H
