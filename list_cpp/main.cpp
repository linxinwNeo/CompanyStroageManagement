#include "mainwindow.h"
#include "entrylist.h"
#include "client_info.h"

#include <QApplication>

#include <QDebug>

QString filename = "mypdf.pdf";

EntryList EL;
Client_info client_info;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/icon.png"));

    MainWindow w;
    w.show();
    return a.exec();
}


