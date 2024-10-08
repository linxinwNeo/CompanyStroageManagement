
#include "create_pdf.h"

int number_of_characters_of_description = 26;

QString currDate(QSharedPointer<QDateTime> date)
{
    if(date.isNull()) return QDateTime().currentDateTime().toString("dd MMM yyyy");
    else return date->toString("dd MMM yyyy");
}

QString currTime(QSharedPointer<QDateTime> time)
{
    if(time.isNull()) return QDateTime().currentDateTime().toString("hh:mm:ss");
    else return time->toString("hh:mm:ss");
}


// make the pdf given the information saved in private variable <this->list>
void create_pdf(QString filename, ListPtr list)
{
    // setting up the pdf format
    QPdfWriter pdf_file(filename);

    pdf_file.setPageSize(QPageSize::A4);
    pdf_file.setPageMargins(QMargins(300, 300, 300, 300));

    const auto& client_info = list->client_info;
    const auto& items = list->entryList;

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

    QFont non_bold("Verdana", 8);

    painter.setFont(bold);
    painter.setPen(pen);

    QLocale locale(QLocale::English, QLocale::UnitedStates);

    // CLIENTE
    painter.drawText(QPointF(width * 0.13, height * 0.172), client_info.m_clientName);

    // AGENTE
    painter.drawText(QPointF(width * 0.78, height * 0.168), client_info.m_ID);

    // FECHA
    painter.drawText(QPointF(width * 0.78, height * 0.19), currDate(list->datetime_created));

    // DOMICILIO
    painter.drawText(QPointF(width * 0.13, height * 0.212), client_info.m_DOMICILIO);

    // CIUDAD
    painter.drawText(QPointF(width * 0.13, height * 0.23), client_info.m_CIUDAD);

    // Hora
    painter.drawText(QPointF(width * 0.78, height * 0.229), currTime(list->datetime_created));

    // RFC
    painter.drawText(QPointF(width * 0.13, height * 0.2475), client_info.m_RFC);

    // CONDICIONES
    painter.drawText(QPointF(width * 0.78, height * 0.25), client_info.m_CONDICIONES);

    double y = height * 0.31;

    for(auto& entry : items.entries){
        double x = width * 0.06; // reset x for each entry

        // CAJA
        painter.drawText(QPointF(x, y), locale.toString(entry->num_boxes(), 'f', 2));
        x += width * 0.07;

        // CANTIDAD
        painter.drawText(QPointF(x, y), locale.toString(entry->NUM_PIECES));
        x += width * 0.087;

        // CANT POR CAJA
        painter.drawText(QPointF(x, y), locale.toString(entry->NUM_PIECES_PER_BOX));
        x += width * 0.08;

        // CLAVE
        painter.drawText(QPointF(x, y), entry->MODEL_CODE);
        x += width * 0.122;

        // DESCRIPTION
        if(entry->Description_SPAN.size() <= number_of_characters_of_description){
            painter.drawText(QPointF(x, y), entry->Description_SPAN);
        }
        else{
            // 字符串太长了，需要取前一部分，并留 "..." 在末尾
            painter.drawText(QPointF(x, y),
                             entry->Description_SPAN.left( number_of_characters_of_description ) + "..."
                             );
        }
        x += width * 0.273;

        // PRECIO U.
        painter.drawText(QRect(0, y - height*0.009, x + width*0.116, y - height*0.009), locale.toString(entry->PRICE_PER_PIECE, 'f', 2), option);
        x += width * 0.129;

        // IMPORTE
        painter.drawText(QRect(0, y - height*0.009, x + width*0.13, y - height*0.009), locale.toString(entry->TOTAL, 'f', 2), option);

        // increment y
        y += height * 0.02;
    }

    // compute totals
    double subtotal, total;
    list->total(subtotal, total);

    // SUBTOTAL
    painter.drawText(QRect(0, height * 0.815, width * 0.95, height * 0.815), locale.toString(subtotal, 'f', 2), option);

    // DESCUENTO
    painter.drawText(QRect(0, height * 0.84, width * 0.95, height * 0.84), locale.toString(subtotal - total, 'f', 2), option);

    // IVA
    painter.drawText(QRect(0, height * 0.875, width * 0.95, height * 0.875), "0.00", option);

    // TOTAL
    painter.drawText(QRect(0, height * 0.907, width * 0.95, height * 0.907), locale.toString(total, 'f', 2), option);

    // the total number of boxes
    painter.drawText(QPointF(width * 0.07, height * 0.91), locale.toString(client_info.m_TOTAL_NUM_BOXES, 'f', 2));

    // the discount percent using thinner font
    painter.setFont(non_bold);
    painter.drawText(QRect(0, height * 0.84, width * 0.8, height * 0.84), "(" + QString::number(client_info.m_DISCOUNT * 100.) + "%)", option);

    // cover the old id with white rect
    painter.setBrush(Qt::SolidPattern);
    painter.fillRect(QRect(width * 0.18, height * 0.92,
                           width * 0.19, height * 0.93), QBrush(Qt::white));

    // draw thick red id
    pen.setWidth(15);
    pen.setColor(Qt::red);
    QFont thick_bold("Verdana", 15);
    thick_bold.setBold(true);
    painter.setFont(thick_bold);
    painter.setPen(pen);
    painter.drawText(QPointF(width * 0.18, height * 0.948), QString::number(list->id));

    painter.end();
}
