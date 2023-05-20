#include "mainwindow.h"
#include <QApplication>
#include "GlobalVars.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon.png"));

    MainWindow w;
//    QXlsx::Document xlsx;
//    xlsx.write("A1", "Hello Qt!"); // write "Hello Qt!" to cell(A,1). it's shared string.
//    xlsx.saveAs("/Users/linxinw/Desktop/test.xlsx"); // save the document as 'Test.xlsx'

    // setting up the window
    w.setWindowTitle(APP_NAME);
    w.show();
    return a.exec();
}


