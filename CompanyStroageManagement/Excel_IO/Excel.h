#ifndef EXCEL_H
#define EXCEL_H

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include "mainwindow.h"
#include "QXlsx/QXlsxHeaders.h"

class Excel
{
public:
    QString fpath;

    Excel();
    Excel(const QString);
    ~Excel();

    void read_excel();
};

#endif // EXCEL_H
