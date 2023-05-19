#include "mainwindow.h"
#include <QApplication>
#include "GlobalVars.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    // setting up the window
    w.setWindowIcon(QIcon(":/imgs/resources/icon.png"));
    w.setWindowTitle(APP_NAME);
    w.show();
    return a.exec();
}


