#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QString>

#include "Predefined.h"
#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"
#include "Excel_IO/Excel.h"

extern EntryList EL;
extern Client_info client_info;
extern QString stroagefilePath;
extern const QString APP_NAME;
extern Excel* excel;


extern const QString APP_NAME;
extern const QString FATAL_ERROR_MESSAGE;
extern const QString EXCEL_CAPTION;
extern const QString EXCEL_FILTER;
extern const QString EXCEL_DEFAULT_DIR;
extern const QString EXCEL_SELECT_A_VALID_XLSX_FILE;

#endif // GLOBALVARS_H
