#include "CreateListWin.h"
#include "ui_CreateList.h"

#include "helper_functions.h"
#include "GlobalVars.h"
#include "flags.h"
#include "QXlsx/Excel.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "FileLoader/ReadFile.h"

CreateListWin::CreateListWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateList)
{
    ui->setupUi(this);

    // setting up the table
    this->table = ui->tableWidget;
    table->setRowCount(0);
    table->setColumnCount(7);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

CreateListWin::~CreateListWin()
{
    delete ui;
}


void CreateListWin::on_generatePDF_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, PDF_MESSAGE_1, PDF_MESSAGE_2,
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }


    // save client info first
    client_info.CLIENTE = this->ui->CLIENTE_LE->text();
    client_info.DOMICILIO = this->ui->DOMICILIO_LE->text();
    client_info.CIUDAD = this->ui->CIUDAD_LE->text();
    client_info.RFC = this->ui->RFC_LE->text();
    client_info.AGENTE = this->ui->AGENTE_LE->text();
    client_info.CONDICIONES = this->ui->CONDICIONES_LE->text();
    qDebug() << "DISCOUNT" << client_info.DISCOUNT;
    client_info.DISCOUNT = this->ui->discount_SB->value();
    client_info.bottom_left_num = this->ui->btm_left_num_SB->value();

    // ask for the path to store the file
    QString filter = tr("PDF (*.pdf)");
    QString filename = QFileDialog::getSaveFileName(this, GET_DESTINATION_MESSAGE, "list", filter);
    if(filename.isEmpty())
        return;

    QMessageBox Msgbox(this);
    if(!is_preview_list){
        // deduct items in the stroage
//        inventory.deduct();
    }


    // create PDF file
    this->create_pdf(filename);


    // display creation success file
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox.setText(".pdf 文件创建成功");
    Msgbox.exec();
}


void CreateListWin::closeEvent (QCloseEvent *event)
{
    QMessageBox msg(this);
    msg.setText(tr("你确定要退出吗?\n"));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");

    int resBtn = msg.exec();
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    }
    else {
        event->accept();
    }
}

