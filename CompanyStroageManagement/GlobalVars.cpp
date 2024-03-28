#include "GlobalVars.h"

const QString APP_NAME = "Yi Hong Ding 库存管理软件";

const QString BackUP_DirName = "./backUps";
const QString BackUP_FileName = "BackUpDate.txt";
const unsigned int backup_every_n_days = 1;

const QString Settings_FileName = "settings.txt";

class Inventory inventory; // holds all models, containers
class Lists lists; // holds all past list

const QString EXCEL_FILTER = "Microsoft Excel Open XML Spreadsheet (*.xlsx)";
const QString table_stylesheet = "QHeaderView::section { background-color:#F0F0F0; }";

const QString DateTimeFormat = "yyyy-MM-dd HH:mm:ss";
const QString DateTimeFormat_for_backup_file = "yyyy-MM-dd-HH-mm-ss";

unsigned int language_option = 0; // 0 for chinese, 1 for Spanish


QLocale locale(QLocale::English, QLocale::UnitedStates);
