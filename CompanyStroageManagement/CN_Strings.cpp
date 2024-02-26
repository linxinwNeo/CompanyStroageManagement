#include "CN_Strings.h"

// window titles
const QString AddNewModel_WinTitle_CN = "新增一个新的货物";
const QString CreateList_WinTitle_CN = "创建一个新的清单";
const QString Search_List_WinTitle_CN = "搜索清单";

// Error and Warning Messages
const QString FATAL_ERROR_MSG_CN = "出现无法解决的错误，请联系制作者。";
const QString UNABLE_OPEN_INVENTORY_FILE_MSG_CN = "无法打开库存文件, 请在应用启动后手动选择库存文件";
const QString UNABLE_OPEN_LISTS_FILE_MSG_CN = "无法打开清单历史文件, 请在应用启动后手动选择清单文件";
const QString UNABLE_TO_SAVE_BACKUP_DATA_MSG_CN = "保存备份文件失败！";
const QString SAVE_ERROR_MSG_CN = "保存失败, 关闭已经打开的文件并且再次保存可能解决该问题";
const QString ADD_NEW_MODEL_FAIL_MSG_CN = "添加失败！";

const QString EXCEL_CAPTION_CN = "选择库存文件";
const QString EXCEL_SELECT_A_VALID_XLSX_FILE_CN = "你必须选择一个有效的XLSX文件，软件现在将退出。";
const QString PDF_MESSAGE_1_CN = "是否确定生成清单";
const QString PDF_MESSAGE_2_CN = "确定生成清单？";
const QString LIST_CREATED_MSG_CN = "清单创建成功!";
const QString WHERE_TO_SAVE_FILE_MESSAGE_CN = "想将文件保存在哪里？";
const QString MODELCODE_EMPTY_ERROR_CN = "货号不能为空！";
const QString duplicate_Model_ERROR_MSG_CN = "该货号已存在！";
const QString SOLD_MORETHAN_INIT_BOXES_ERROR_MSG_CN = "出售箱数应该小于初始箱数！";
const QString NEW_MODEL_ADD_SUCCESS_MSG_CN = "新货物已成功添加";
const QString DELETE_MODEL_COMFIRMATION_MSG_PREFIX_CN = "你确定要删除货号为";
const QString DELETE_MODEL_COMFIRMATION_MSG_SUFFIX_CN = "的货物吗?";

const QString DELETE_SUCCESS_MSG_CN = "删除成功！";
const QString READ_SUCCESS_MSG_CN = "读取成功！";
const QString READ_FAIL_MSG_CN = "读取失败！";
const QString Are_You_Sure_to_Exit_CN = "你确定要关闭程序吗?";
const QString Are_You_Sure_to_Update_CN = "你确定要更改这个货物吗";
const QString SAVE_SUCCESS_MSG_CN = "保存成功！";
const QString OUT_OF_STOCK_MSG_CN = "该货没有库存了!";
const QString MODEL_EXIST_IN_LIST_MSG_CN = "该货已经在清单中了.";
const QString EMPTY_LIST_CN = "清单是空的.";

/*
void QMessageBox_Example(){
    QMessageBox* msgBox = new QMessageBox;
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle("Information");
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setText(lan(UNABLE_OPEN_LIST_FILE_MSG_CN, UNABLE_OPEN_LIST_FILE_MSG_SPAN));
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->exec();
}
*/
