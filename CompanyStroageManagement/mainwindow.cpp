#include "mainwindow.h"
#include "helper_functions.h"
#include "ui_mainwindow.h"
#include "GlobalVars.h"
#include "flags.h"
#include "QXlsx/Excel.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "FileLoader/readTxt.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setting up the table
    this->table = ui->tableWidget;
    table->setRowCount(0);
    table->setColumnCount(7);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // read inventory.txt file
    ReadTxt tet;
    tet.read_Models(DB_FNAME);
}


MainWindow::~MainWindow()
{
    delete ui;

    excel = nullptr;
}



void MainWindow::on_generatePDF_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "是否确定生产pdf文件", "确定生成pdf文件？",
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
    QString filename= QFileDialog::getSaveFileName(this, "where do you want to place", "list", filter);
    if(filename.isEmpty())
        return;

    QMessageBox Msgbox(this);
    if(!is_preview_list){
        // deduct items in the stroage
        inventory.deduct();
    }


    // create PDF file
    this->create_pdf(filename);


    // display creation success file
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox.setText(".pdf 文件创建成功");
    Msgbox.exec();
}


void MainWindow::closeEvent (QCloseEvent *event)
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

