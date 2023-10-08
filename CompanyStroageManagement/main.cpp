#include "FileLoader/ReadFile.h"
#include "mainwindow.h"
#include <QApplication>
#include "GlobalVars.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon.icns"));

    // read settings file
    ReadFile rf;
    rf.read_settings_file();
    // read inventory.txt file
    rf.read_Inventory_xlsx_File(Inventory_FNAME_xlsx);
    // read lists.txt file
    rf.read_Lists_txt_File(Lists_FNAME); // build the lists

    MainWindow w;
    // setting up the window
    w.setWindowTitle(APP_NAME);

    w.showMaximized();

    return a.exec();
}


