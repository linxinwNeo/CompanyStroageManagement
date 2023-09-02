#include "mainwindow.h"
#include <QApplication>
#include "GlobalVars.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon.icns"));

    MainWindow w;
    // setting up the window
    w.setWindowTitle(APP_NAME);
    w.showMaximized();

    return a.exec();
}


