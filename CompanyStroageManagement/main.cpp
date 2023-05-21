#include "mainwindow.h"
#include <QApplication>
#include "GlobalVars.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon.ico"));

    MainWindow w;
    // setting up the window
    w.setWindowTitle(APP_NAME);
    w.show();
    return a.exec();
}


