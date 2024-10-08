#include "GlobalVars.h"

Inventory inventory; // holds all models, containers
ListManager listManager; // managers lists
ClientManager clientManager; // manages clients

QString GlobalVars::APP_NAME = "Yi Hong Ding 库存管理软件";

QString GlobalVars::BackUP_DirName = "./backUps";
QString GlobalVars::BackUP_FileName = "BackUpDate.txt";
unsigned int GlobalVars::backup_every_n_hours = 24;

QString GlobalVars::Lists_DirName = "ListsRecords";

QString GlobalVars::Clients_FileName = "Clients.txt";

QString GlobalVars::Settings_FileName = "settings.txt";

QString GlobalVars::EXCEL_FILTER = "Microsoft Excel Open XML Spreadsheet (*.xlsx)";

QString GlobalVars::DateTimeFormat = "yyyy-MM-dd HH:mm:ss";
QString GlobalVars::DateTimeFormat_year = "yyyy";
QString GlobalVars::DateTimeFormat_month = "MM";
QString GlobalVars::DateTimeFormat_for_file = "yyyy-MM-dd-HH-mm-ss";

unsigned int GlobalVars::language_option = 0; // 0 for chinese, 1 for Spanish

QLocale GlobalVars::locale(QLocale::English, QLocale::UnitedStates);

QString GlobalVars::cur_password = "111111"; // default password
QString GlobalVars::backDoor_password = "lin@88898909"; // this password should always work


QStringList GlobalVars::table_headers_model(){
    return{
        lan("货号", "MODELO"),
        lan("集装箱号", "Número de contenedor"),
        lan("品名(中文)", "Nombre del producto (en chino)"),
        lan("品名(西语)", "Nombre del producto (en español)"),

        lan("进货箱数", "ENTRADAS"),
        lan("已售箱数", "SALIDAS"),
        lan("剩余箱数", "CAJA"),

        lan("每箱个数", "CANT POR CAJA"),

        lan("进货个数", "Núm. piezas adq."),
        lan("已售个数", "Núm. piezas vend."),
        lan("剩余个数", "EXISTENCIA"),

        lan("单价($)", "PRECIO U.($)"),
        lan("上次修改时间", "Fecha últ. modif."),
        lan("进货时间", "Tiempo de stock")
    };
};

QStringList GlobalVars::table_headers_client()
{
    return {
        lan("代理", "AGENTE"),
        lan("客户名字", "CLIENTE"),
        lan("联系地址", "DOMICILIO"),
        lan("城市", "CIUDAD"),
        lan("R.F.C.", "R.F.C."),
        lan("付款方式", "CONDICIONES DE PAGO")
    };
};

QStringList GlobalVars::table_headers_container(){
    return {
        lan("集装箱号", "Número de contenedor"),
        lan("货物种类数量", "Núm. categorías merc."),
        lan("进货箱数", "ENTRADAS"),
        lan("剩余箱数", "CAJA"),
        lan("进货个数", "Núm. piezas adq."),
        lan("剩余个数", "Núm. piezas rest.")
    };
};
