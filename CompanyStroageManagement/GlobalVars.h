#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QString>
#include <QSharedPointer>

#include "Predefined.h"
#include "DataStructures/Inventory.h"
#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"
#include "QXlsx/Excel.h"

extern EntryList EL_add;
extern EntryList EL_deduct;

extern QString DB_FNAME;

extern Client_info client_info;
extern QString stroagefilePath;
extern const QString APP_NAME;
extern QSharedPointer<Excel> excel;
extern Inventory inventory;

extern const QString APP_NAME;
extern const QString FATAL_ERROR_MESSAGE;
extern const QString EXCEL_CAPTION;
extern const QString EXCEL_FILTER;
extern const QString EXCEL_DEFAULT_DIR;
extern const QString EXCEL_SELECT_A_VALID_XLSX_FILE;

extern const QString PDF_MESSAGE_1;
extern const QString PDF_MESSAGE_2;
extern const QString GET_DESTINATION_MESSAGE;
extern const QString table_stylesheet;
#endif // GLOBALVARS_H
