#include "FileLoader/ReadFile.h"
#include "mainwindow.h"
#include <QApplication>
#include "GlobalVars.h"
#include <QMessageBox>
#include "LanguageStrings.h"

void readfile();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon.icns"));

    readfile();

    MainWindow w;

    // setting up the window
    w.setWindowTitle(APP_NAME);

    w.show();

    return a.exec();
}


void readfile()
{
    // try to read settings file
    ReadFile::read_settings_file();

    // try to read inventory file
    if( !ReadFile::read_Inventory_File_Auto(false) ) {
        QMessageBox* msgBox = new QMessageBox;
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(lan(UNABLE_OPEN_INVENTORY_FILE_MSG_CN, UNABLE_OPEN_INVENTORY_FILE_MSG_SPAN));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }

    // try to read lists file
    if( !ReadFile::read_Lists_txt_File(false) ){
        QMessageBox* msgBox = new QMessageBox;
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(lan(UNABLE_OPEN_LISTS_FILE_MSG_CN, UNABLE_OPEN_LISTS_FILE_MSG_SPAN));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }
}


