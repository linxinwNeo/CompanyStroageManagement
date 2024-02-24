#include "FileLoader/ReadFile.h"
#include "mainwindow.h"
#include <QApplication>
#include "GlobalVars.h"

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
    // read settings file
    ReadFile rf;

    rf.read_settings_file();

    // read inventory.xlsx file
    rf.read_Inventory_File_Auto();

    // read lists.txt file
    rf.read_Lists_txt_File(Lists_FNAME); // build the lists
}


