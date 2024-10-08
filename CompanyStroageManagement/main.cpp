#include <QApplication>
#include <QMessageBox>

#include "FileLoader/ReadFile.h"
#include "mainwindow.h"
#include "GlobalVars.h"
#include "LanguageStrings.h"
#include "AuthenticationWin.h"

void read_files();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon.icns"));

    // try to read settings file
    ReadFile::read_settings_file();

    // verify user
    AuthenticationWin auth_win;
    auth_win.setWindowTitle(lan("密码确认", "Confirmación de contraseña"));
    while(auth_win.exec()){
        if(auth_win.password_correct){
            break;
        }
    }

    if(!auth_win.password_correct){
        return 0;
    }

    read_files();

    MainWindow w;

    // setting up the window
    w.setWindowTitle(GlobalVars::APP_NAME);

    w.show();

    return a.exec();
}


// read inventory and clients
void read_files()
{
    // try to read inventory file
    if( !ReadFile::read_Inventory_File_Auto(false) ) {
        QMessageBox* msgBox = new QMessageBox;
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(lan(UNABLE_OPEN_INVENTORY_FILE_MSG_CN, UNABLE_OPEN_INVENTORY_FILE_MSG_SPAN));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }

    // try to read clients file
    ReadFile::Read_Clients();

    listManager.update_max_id();
}


