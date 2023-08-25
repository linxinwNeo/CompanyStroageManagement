#ifndef CREATE_PDF_H
#define CREATE_PDF_H

#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QDate>
#include <QTime>
#include <QFile>

#include "GlobalVars.h"
#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"


QString currDate();
QString currTime();
void create_pdf(QString filename, ListPtr list);

#endif // CREATE_PDF_H
