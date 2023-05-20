#ifndef EXCEL_H
#define EXCEL_H

#include "GlobalVars.h"


class Excel
{
public:
    QString fpath;

    Excel(const QString);
    ~Excel();

    void read_excel(); //TODO
};

#endif // EXCEL_H
