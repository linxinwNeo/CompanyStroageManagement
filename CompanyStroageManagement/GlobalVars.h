#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QString>
#include <QSharedPointer>
#include "Predefined.h"
#include "DataStructures/Inventory.h"
#include "DataStructures/client_info.h"
#include "QXlsx/QXlsxHeaders.h"
#include "DataStructures/List.h"

extern QString Inventory_FNAME_xlsx;
extern QString Lists_FNAME;

extern const QString BackUP_DirName;
extern const QString BackUP_FileName;

extern QString stroagefilePath;
extern const QString APP_NAME;
extern Inventory inventory;
extern Lists lists;
extern const QString AddNewModel_WinTitle;
extern const QString CreateList_WinTitle;
extern const QString Search_List_WinTitle;

extern const QString UNABLE_TO_SAVE_BACKUP_DATE_MSG;

extern const QString APP_NAME;
extern const QString FATAL_ERROR_MSG;
extern const QString SAVE_ERROR_MSG;
extern const QString UNABLE_OPEN_FILE_MSG;
extern const QString EXCEL_CAPTION;
extern const QString EXCEL_FILTER;
extern const QString EXCEL_DEFAULT_DIR;
extern const QString EXCEL_SELECT_A_VALID_XLSX_FILE;

extern const QString PDF_MESSAGE_1;
extern const QString PDF_MESSAGE_2;
extern const QString WHERE_TO_SAVE_FILE_MESSAGE;
extern const QString table_stylesheet;

extern const QString empty_MODELCODE_ERROR_MSG;
extern const QString duplicate_Model_ERROR_MSG;
extern const QString SOLD_MORETHAN_INIT_BOXES_ERROR_MSG;
extern const QString NEW_MODEL_ADD_SUCCESS_MSG;
#endif // GLOBALVARS_H
