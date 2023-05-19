#ifndef EXCEL_H
#define EXCEL_H

#include <QObject>
#include <QAxObject>
#include <QStandardPaths>

#include "GlobalVars.h"


class Excel
{
public:
    QAxObject* axobj;
    QString fpath;

    Excel(const QString);
    ~Excel();

    void read_excel(); //TODO
};

#endif // EXCEL_H
