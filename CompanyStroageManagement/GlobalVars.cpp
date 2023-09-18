#include "GlobalVars.h"

QString stroagefilePath;

QString Inventory_FNAME_xlsx = "inventory.xlsx";
QString Lists_FNAME = "lists.txt";

const QString BackUP_DirName = "./backUps";
const QString BackUP_FileName = "BackUpDate.txt";

class Inventory inventory; // holds all models, containers
class Lists lists; // holds all past list

const QString APP_NAME = "Yi Hong Ding 库存管理软件";
const QString FATAL_ERROR_MSG = "出现无法解决的错误，请联系制作者。";
const QString AddNewModel_WinTitle = "新增一个新的货物";
const QString CreateList_WinTitle = "创建一个新的清单";
const QString Search_List_WinTitle = "搜索清单";

const QString UNABLE_TO_SAVE_BACKUP_DATE_MSG = "保存备份文件失败！";

const QString SAVE_ERROR_MSG = "保存失败, 关闭已经打开的文件并且再次保存可能解决该问题";
const QString UNABLE_OPEN_FILE_MSG = "无法打开文件";

const QString EXCEL_CAPTION = "选择库存文件";
const QString EXCEL_FILTER = "Microsoft Excel Open XML Spreadsheet (*.xlsx)";
const QString EXCEL_DEFAULT_DIR = "stroage.xlsx";
const QString EXCEL_SELECT_A_VALID_XLSX_FILE = "你必须选择一个有效的XLSX文件，软件现在将退出。";

const QString PDF_MESSAGE_1 = "是否确定生产pdf文件";
const QString PDF_MESSAGE_2 = "确定生成pdf文件？";

const QString WHERE_TO_SAVE_FILE_MESSAGE = "想将文件保存在哪里？";

const QString table_stylesheet = "QHeaderView::section { background-color:#F0F0F0; }";

const QString empty_MODELCODE_ERROR_MSG = "货号不能为空！";
const QString duplicate_Model_ERROR_MSG = "该货号已存在！";
const QString SOLD_MORETHAN_INIT_BOXES_ERROR_MSG = "出售箱数应该小于初始箱数！";
const QString NEW_MODEL_ADD_SUCCESS_MSG = "新货物已成功添加";
