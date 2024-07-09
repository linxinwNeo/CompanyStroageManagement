#include <mainwindow.h>
#include "GlobalVars.h"
#include "FileLoader/WriteFile.h"

// change language to Spanish
void MainWindow::on_actionSpanish_triggered()
{
    GlobalVars::language_option = 1; // spanish is 1
    this->setLanguage();
}


// change language to Chinese
void MainWindow::on_actionChinese_triggered()
{
    GlobalVars::language_option = 0; // chinese is 0
    this->setLanguage();
}


/* 2/24/2024
 * Save Inventory file to the path that we opened it last time.
 */
void MainWindow::on_action_Save_Inventory_triggered()
{
    if(WriteFile::SaveInventoryAuto(false))
    {
        QMessageBox msg(this);
        msg.setText(lan("保存成功", "Salvar con éxito"));
        msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        msg.exec();
        return;
    }
    else{
        QMessageBox msg(this);
        msg.setText(lan("保存失败", "no salvar"));
        msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        msg.exec();
        return;
    }
}


// 改密码
void MainWindow::on_actionChangePassword_triggered()
{
    this->ChangePasswordWinPtr.setWindowTitle(lan("改密码","Cambia tu contraseña"));
    this->ChangePasswordWinPtr.show();
    this->hide();
}


