#include <mainwindow.h>
#include "GlobalVars.h"
#include "FileLoader/WriteFile.h"

// change language to Spanish
void MainWindow::on_actionSpanish_triggered()
{
    language_option = 1; // spanish is 1
    this->setLanguage();
}


// change language to Chinese
void MainWindow::on_actionChinese_triggered()
{
    language_option = 0; // chinese is 0
    this->setLanguage();
}


/* 2/24/2024
 * Save Inventory file to the path that we opened it last time.
 */
void MainWindow::on_action_Save_Inventory_triggered()
{
    WriteFile a;
}


/* 2/24/2024
 * Save Lists file to the path that we opened it last time.
 */
void MainWindow::on_action_Save_Lists_triggered()
{

}


