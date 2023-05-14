#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "client_info.h"
#include "entrylist.h"
#include <QString>

EntryList EL;
Client_info client_info;
const QString APP_NAME = "Yi Hong Ding Stroage Management System";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    // setting up the window
    w.setWindowIcon(QIcon(":/images/icon.png"));
    w.setWindowTitle(APP_NAME);
    w.show();
    return a.exec();
}


