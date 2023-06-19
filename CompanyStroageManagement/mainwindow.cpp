#include "mainwindow.h"
#include "helper_functions.h"
#include "ui_mainwindow.h"
#include "GlobalVars.h"
#include "flags.h"
#include "QXlsx/Excel.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "addbackwindow.h"

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

    win = QSharedPointer<AddbackWindow> ( new AddbackWindow( this ) );

    // check if previous excel exists
    // if no, ask for a excel file
    // if yes, use the previous one
    excel = QSharedPointer<Excel> ( new Excel() );
    excel->read_excel(); // read excel file and create stroage file
    qDebug() << "stroage size" << stroage.size();
}


MainWindow::~MainWindow()
{
    delete ui;

    excel = nullptr;
    win = nullptr;
}


double MainWindow::get_discount_value() const
{
    return (client_info.DISCOUNT * 0.01 * EL_deduct.subtotal());
}

double MainWindow::get_total() const
{
    return EL_deduct.subtotal() - this->get_discount_value();
}


// add_entry btn is released, save everything in the entry form and add one entry on the list
void MainWindow::on_add_entry_btn_released()
{
//    qDebug() << "add_entry_btn_released!";
    QString text = this->ui->PRECIO_LE->text();
    if (!is_float(text)) {
        QMessageBox Msgbox(this);
        Msgbox.setText("Please enter a valid PRECIO U.");
        Msgbox.exec();
        this->ui->PRECIO_LE->setText(QString::number( text.toDouble(), 'f', 2 ));
        return;
    }

    double CAJA = this->ui->CAJA_LE->text().toDouble();
    UL CANTIDAD = this->ui->CANTIDAD_LE->text().toLong();
    UL CANT_POR_CAJA = this->ui->CANT_POR_CAJA_LE->text().toLong();
    QString CLAVE = this->ui->CLAVE_LE->text();
    QString Description = this->ui->DESCRIPTION_LE->text();
    double PRECIO = this->ui->PRECIO_LE->text().toDouble();
    double IMPORTE = this->ui->IMPORTE_LE->text().toDouble();
    double btm_left_num = this->ui->bottom_left_num_LE->text().toDouble();

    QVector<QString> items = {QString::number(CAJA), QString::number(CANTIDAD),
                              QString::number(CANT_POR_CAJA), CLAVE, Description,
                              QString::number(PRECIO), QString::number(IMPORTE),
                              QString::number(btm_left_num)};

    QSharedPointer<Entry> new_entry ( new Entry(CAJA, CANTIDAD, CANT_POR_CAJA,
                                              CLAVE, Description, PRECIO, IMPORTE, btm_left_num) );
    EL_deduct.add_entry( new_entry );

    this->table->insertRow(this->table->rowCount());
    // add entry to the table as well
    UL row = EL_deduct.num_entries() - 1;
    for(int col = 0; col < 8; col++){
        QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
        tableWidgetItem->setText( items[col] );
        table->setItem(row, col, tableWidgetItem);
    }

    // reset entry form
    this->reset_entry_form();
}

void MainWindow::reset_entry_form(){
    this->ui->CAJA_LE->setText("");
    this->ui->CANTIDAD_LE->setText("");
    this->ui->CANT_POR_CAJA_LE->setText("");
    this->ui->CLAVE_LE->setText("");
    this->ui->DESCRIPTION_LE->setText("");
    this->ui->PRECIO_LE->setText("");
    this->ui->IMPORTE_LE->setText("");
}


// reset entry texts
void MainWindow::on_reset_entry_btn_released()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
                                                                tr("你确定要重置此栏所有信息吗?\n"),
                                                                QMessageBox::Yes | QMessageBox::No,
                                                                QMessageBox::Yes);
    if (resBtn == QMessageBox::No) { // return if the user don't want to clear the information
        return;
    }
    this->reset_entry_form();
}


void MainWindow::on_reset_info_btn_released()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
                                                                tr("你确定要重置此栏所有信息吗?\n"),
                                                                QMessageBox::Yes | QMessageBox::No,
                                                                QMessageBox::Yes);
    if (resBtn == QMessageBox::No) { // return if the user don't want to clear the information
        return;
    }

    this->ui->CLIENTE_LE->setText("");
    this->ui->DOMICILIO_LE->setText("");
    this->ui->CIUDAD_LE->setText("");
    this->ui->RFC_LE->setText("");
    this->ui->AGENTE_LE->setText("");
    this->ui->CONDICIONES_LE->setText("");
    this->ui->bottom_left_num_LE->setText("");
    this->ui->DISCOUNT_LE->setText("");
}


// remove all rows
void MainWindow::on_reset_table_btn_released()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
                                                                tr("你确定要清空表格吗?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn == QMessageBox::No) {
        return;
    }

    table->clearContents();
    table->setRowCount(0);
    EL_deduct.clear_memory();
}


void MainWindow::on_delete_row_btn_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
                                                                tr("你确定要删除这（几）行吗?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn == QMessageBox::No) {
        return;
    }

    QItemSelectionModel *select = table->selectionModel();
    if(!select->hasSelection()) return; // check if anything is selected

    // for each row, delete items
    auto num = select->selectedRows().size();
    if(num == 0){
        QMessageBox Msgbox(this);
        Msgbox.setText("没有选中任何一行。请点击要删除的某一行的号码，\n或者同时按住SHIFT或者CTRL进行多选。");
        Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
        Msgbox.exec();
        return;
    }

    for(UL i = 0; i < num; i++) {
        auto first_selected_row = select->selectedRows()[0];
        EL_deduct.remove_entry( first_selected_row.row() );
        table->removeRow( first_selected_row.row() );
    }
}


void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    auto entry = EL_deduct.get_entry(row);
    if(!entry) return;

    switch(column){
    case 0:
        entry->CAJA = this->table->item(row, 0)->text().toDouble();
        break;
    case 1:
        entry->CANTIDAD = this->table->item(row, 1)->text().toDouble();
        break;
    case 2:
        entry->CANT_POR_CAJA = this->table->item(row, 2)->text().toDouble();
        break;
    case 3:
        entry->CLAVE = this->table->item(row, 3)->text();
        break;
    case 4:
        entry->Description = this->table->item(row, 4)->text();
        break;
    case 5:
        entry->PRECIO = this->table->item(row, 5)->text().toDouble();
        break;
    case 6:
        entry->IMPORTE = this->table->item(row, 6)->text().toDouble();
        break;
    default:
        qDebug() << "void MainWindow::on_tableWidget_cellChanged(int row, int column): something is wrong";
    }
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
    client_info.bottom_left_num = this->ui->bottom_left_num_LE->text();
    client_info.DISCOUNT = this->ui->DISCOUNT_LE->text().toDouble();
    qDebug() << "DISCOUNT" << client_info.DISCOUNT;

    if(client_info.DISCOUNT < 0. || client_info.DISCOUNT > 100.){
        QMessageBox Msgbox(this);
        Msgbox.setText("Please enter a valid discount (0.0 - 100.0)");
        Msgbox.exec();
        return;
    }

    // ask for the path to store the file
    QString filter = tr("PDF (*.pdf)");
    QString filename= QFileDialog::getSaveFileName(this, "where do you want to place", "list", filter);
    if(filename.isEmpty())
        return;

    QMessageBox Msgbox(this);
    if(!is_preview_list){
        // deduct items in the stroage
        bool b = stroage.deduct();

        // check if deducting successful
        if(!b){
            Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
            Msgbox.setText("扣除货物失败");
            Msgbox.exec();
            return;
        }
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


void MainWindow::on_is_preview_CB_clicked(bool checked)
{
    is_preview_list = checked;
}


void MainWindow::on_addBack_action_triggered()
{
    if(win.isNull()) win = QSharedPointer<AddbackWindow>  (new AddbackWindow(this));

    win->show();
}


void MainWindow::on_change_excel_action_triggered()
{
    QMessageBox Msgbox(nullptr);
    Msgbox.setText("读取文件中，请稍等");
    Msgbox.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");
    Msgbox.show();

    // delete .txt file
    excel->reset_path();

    excel = nullptr;
    stroage.reset();

    excel = QSharedPointer<Excel> ( new Excel() );
    excel->read_excel(); // read excel file and create stroage file
}

