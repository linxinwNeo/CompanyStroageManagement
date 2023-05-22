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
    void set_value(const QString sheetName, const UI row, const UI col, const QString new_val);
    void reset_path();
};

#endif // EXCEL_H
