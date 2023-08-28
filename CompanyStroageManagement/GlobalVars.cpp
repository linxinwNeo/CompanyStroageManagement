#include "GlobalVars.h"

QString stroagefilePath;
QSharedPointer<Excel> excel (nullptr);

QString DB_FNAME = "C:/Users/linxi/OneDrive/Desktop/inventory.txt";

class Inventory inventory; // holds all models, containers
class Lists lists; // holds all past list

const QString APP_NAME = "Yi Hong Ding 库存管理软件";
const QString FATAL_ERROR_MESSAGE = "出现无法解决的错误，请联系制作者。";
const QString AddNewModel_WinTitle = "新增一个新的货物";
const QString CreateList_WinTitle = "创建一个新的清单";

const QString EXCEL_CAPTION = "选择库存文件";
const QString EXCEL_FILTER = "Microsoft Excel Open XML Spreadsheet (*.xlsx)";
const QString EXCEL_DEFAULT_DIR = "stroage.xlsx";
const QString EXCEL_SELECT_A_VALID_XLSX_FILE = "你必须选择一个有效的XLSX文件，软件现在将退出。";

const QString PDF_MESSAGE_1 = "是否确定生产pdf文件";
const QString PDF_MESSAGE_2 = "确定生成pdf文件？";

const QString GET_DESTINATION_MESSAGE = "想将文件保存在哪里？";

const QString table_stylesheet = "QHeaderView::section { background-color:#F0F0F0; }";
const QString none_CN = "无";
const QString none_SPAN = "nulo";

const QString empty_MODELCODE_ERROR_MSG = "货号不能为空！";
const QString duplicate_Model_ERROR_MSG = "该货号已存在！";
const QString SOLD_MORETHAN_INIT_BOXES_ERROR_MSG = "出售箱数应该小于初始箱数！";
const QString NEW_MODEL_ADD_SUCCESS_MSG = "新货物已成功添加";
