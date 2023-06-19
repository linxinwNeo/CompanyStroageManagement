#include "addbackwindow.h"
#include "ui_addbackwindow.h"
#include "GlobalVars.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>

#include "helper_functions.h"
#include "GlobalVars.h"
#include "DataStructures/EntryList.h"

AddbackWindow::AddbackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddbackWindow)
{
    ui->setupUi(this);

    // setting up the table
    this->table = ui->tableWidget;
    table->setRowCount(0);
    table->setColumnCount(2);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

AddbackWindow::~AddbackWindow()
{
    delete ui;
    EL_add.clear_memory();
    win = nullptr;
}

void AddbackWindow::on_add_entry_btn_clicked()
{
    double CAJA = this->ui->CAJA_LE->text().toDouble();
    QString CLAVE = this->ui->CLAVE_LE->text();

    QSharedPointer<Entry> new_entry (new Entry(CAJA, 0, 0,
                                              CLAVE, "", 0., 0., 0.) );

    EL_add.add_entry(new_entry);
    this->table->insertRow(this->table->rowCount());

    QVector<QString> items = { CLAVE, QString::number(CAJA) };
    // add entry to the table as well
    UL row = EL_add.num_entries()-1;
    for(int col = 0; col < 2; col++){
        QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
        tableWidgetItem->setText( items[col] );
        table->setItem(row, col, tableWidgetItem);
    }

    // reset entry form
    this->reset_entry_form();
}


void AddbackWindow::reset_entry_form(){
    this->ui->CAJA_LE->setText("");
    this->ui->CLAVE_LE->setText("");
}


void AddbackWindow::closeEvent(QCloseEvent *event)
{

}


void AddbackWindow::on_reset_entry_btn_clicked()
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


void AddbackWindow::on_addback_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "是否确定添加这些货物", "确定添加这些货物到库存吗？",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    // deduct items in the stroage
    bool b = stroage.addBack();

    // check if deducting successful
    QMessageBox Msgbox(this);
    if(b){
        // display creation success file
        Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
        Msgbox.setText("添加成功");
        Msgbox.exec();
    }
    else{
        Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
        Msgbox.setText("添加货物失败");
        Msgbox.exec();
    }
}


void AddbackWindow::on_delete_row_btn_clicked()
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

    for(UL i = 0; i<num; i++){
        auto first_selected_row = select->selectedRows()[0];
        EL_add.remove_entry( first_selected_row.row() );
        table->removeRow( first_selected_row.row() );
    }
}


void AddbackWindow::on_reset_table_btn_clicked()
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
    EL_add.clear_memory();
}


void AddbackWindow::on_tableWidget_cellChanged(int row, int column)
{
    auto entry = EL_add.get_entry(row);
    if(!entry) return;

    switch(column){
    case 0:
        entry->CLAVE = this->table->item(row, 0)->text();
        break;
    case 1:
        entry->CAJA = this->table->item(row, 1)->text().toDouble();
        break;
    default:
        qDebug() << "void AddbackWindow::on_tableWidget_cellChanged(int row, int column): something is wrong";
    }
}

