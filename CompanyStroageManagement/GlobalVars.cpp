#include "GlobalVars.h"

EntryList EL_deduct;
EntryList EL_add;
Client_info client_info;
QString stroagefilePath;
QSharedPointer<Excel> excel (nullptr);

QString DB_FNAME = "C:/Users/linxi/OneDrive/Desktop/inventory.txt";

class Inventory inventory;

const QString APP_NAME = "Yi Hong Ding 库存管理软件";
const QString FATAL_ERROR_MESSAGE = "出现无法解决的错误，请联系制作者。";

const QString EXCEL_CAPTION = "选择库存文件";
const QString EXCEL_FILTER = "Microsoft Excel Open XML Spreadsheet (*.xlsx)";
const QString EXCEL_DEFAULT_DIR = "stroage.xlsx";
const QString EXCEL_SELECT_A_VALID_XLSX_FILE = "你必须选择一个有效的XLSX文件，软件现在将退出。";

const QString PDF_MESSAGE_1 = "是否确定生产pdf文件";
const QString PDF_MESSAGE_2 = "确定生成pdf文件？";

const QString GET_DESTINATION_MESSAGE = "想将文件保存在哪里？";
