#include <QMessageBox>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QDate>
#include <QTime>
#include <QFile>

#include "GlobalVars.h"
#include "CreateList.h"
#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"

QString CreateList::currDate() const
{
    QDate date = QDate::currentDate();
    return date.toString("dd MMM yyyy");
}

QString CreateList::currTime() const
{
    QTime time = QTime::currentTime();
    return time.toString("hh:mm:ss");
}

void CreateList::create_pdf(QString filename) {
    // setting up the pdf format
    QPdfWriter pdf_file(filename);
    pdf_file.setPageSize(QPageSize::A4);
    pdf_file.setPageMargins(QMargins(300, 300, 300, 300));

    // setting up the painter
    QPainter painter(&pdf_file);
    // get the dimension of the viewport
    int width = painter.viewport().width();
    int height = painter.viewport().height();


    // set background as the template
    painter.drawImage(QRect(0, 0, width, height), QImage(":/resources/template.png"));

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(5);
    QFont bold("Verdana", 8);
    bold.setBold(true);
    QTextOption  option;
    option.setTextDirection(Qt::RightToLeft);

    painter.setFont(bold);
    painter.setPen(pen);

    // CLIENTE
    painter.drawText(QPointF(width * 0.13, height * 0.172), client_info.CLIENTE);
    qDebug() << "CLIENTE" << client_info.CLIENTE;

    // AGENTE
    painter.drawText(QPointF(width * 0.78, height * 0.168), client_info.AGENTE);

    // FECHA
    painter.drawText(QPointF(width * 0.78, height * 0.19), this->currDate());

    // DOMICILIO
    painter.drawText(QPointF(width * 0.13, height * 0.212), client_info.DOMICILIO);

    // CIUDAD
    painter.drawText(QPointF(width * 0.13, height * 0.23), client_info.CIUDAD);

    // Hora
    painter.drawText(QPointF(width * 0.78, height * 0.229), this->currTime());

    // RFC
    painter.drawText(QPointF(width * 0.13, height * 0.2475), client_info.RFC);

    // CONDICIONES
    painter.drawText(QPointF(width * 0.78, height * 0.25), client_info.CONDICIONES);

    double y = height * 0.31;
    qDebug() << EL_deduct.size();
    for(auto entry : EL_deduct.entries){
        double x = width * 0.06; // reset x for each entry

        // CAJA
        painter.drawText(QPointF(x, y), QString::number(entry->CAJA));
        x += width * 0.07;

        // CANTIDAD
        painter.drawText(QPointF(x, y), QString::number(entry->CANTIDAD));
        x += width * 0.087;

        // CANT POR CAJA
        painter.drawText(QPointF(x, y), QString::number(entry->CANT_POR_CAJA));
        x += width * 0.08;

        // CLAVE
        painter.drawText(QPointF(x, y), entry->CLAVE);
        x += width * 0.122;

        // DESCRIPTION
        painter.drawText(QPointF(x, y), entry->Description);
        x += width * 0.273;

        // PRECIO U.
        painter.drawText(QRect(0, y - height*0.009, x + width*0.116, y - height*0.009), QString::number((double)entry->PRECIO, 'f', 2), option);
        x += width * 0.129;

        // IMPORTE
        painter.drawText(QRect(0, y - height*0.009, x + width*0.13, y - height*0.009), QString::number((double)entry->IMPORTE, 'f', 2), option);

        // increment y
        y += height * 0.02;
    }

    // SUBTOTAL
    painter.drawText(QRect(0, height * 0.815, width * 0.93, height * 0.815), QString::number(EL_deduct.subtotal(), 'f', 2), option);

    // DESCUENTO
    painter.drawText(QRect(0, height * 0.84, width * 0.93, height * 0.84), QString::number(this->get_discount_value(), 'f', 2), option);
    painter.drawText(QRect(0, height * 0.84, width * 1, height * 0.84), "(" + QString::number(client_info.DISCOUNT) + "%)", option);

    // IVA
    painter.drawText(QRect(0, height * 0.875, width * 0.93, height * 0.875), "0.00", option);

    // TOTAL
    painter.drawText(QRect(0, height * 0.907, width * 0.93, height * 0.907), QString::number(this->get_total(), 'f', 2), option);

    // bottom left num
    painter.drawText(QPointF(width * 0.07, height * 0.91), QString::number(client_info.bottom_left_num));

    painter.end();
}
