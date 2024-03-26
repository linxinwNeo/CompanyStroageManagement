#ifndef CREATE_PDF_H
#define CREATE_PDF_H

#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime>
#include <QFile>

#include "GlobalVars.h"
#include "DataStructures/client_info.h"
#include "DataStructures/Entrylist.h"


QString currDate(const QDate date);
QString currTime(const QTime time);
void create_pdf(QString filename, ListPtr list);

#endif // CREATE_PDF_H
