#include "Search_List_Win.h"
#include "CN_Strings.h"
#include "FileLoader/WriteFile.h"
#include "GlobalVars.h"
#include "SpanStrings.h"
#include "ui_Search_List_Win.h"

#include <QMessageBox>
#include <QCloseEvent>
#include "mainwindow.h"

Search_List_Win::Search_List_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search_List_Win)
{
    ui->setupUi(this);

    this->setLanguage();

    searched_lists_table = ui->searched_lists_table;
//    searched_lists_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    searched_lists_table->setStyleSheet(table_stylesheet);

    list_models_table = ui->list_models_table;
//    list_models_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    list_models_table->setStyleSheet(table_stylesheet);

    this->on_list_id_2be_searched_LE_textChanged(this->ui->list_id_2be_searched_LE->text());
}


Search_List_Win::~Search_List_Win()
{
    delete ui;
}


void Search_List_Win::set_parentWin(MainWindow* win)
{
    this->parentWin = win;
}


void Search_List_Win::view_selected_list()
{
    if(this->selected_list.isNull()) return;

    this->ui->CLIENTE_LE->setText(selected_list->client_info.CLIENTE);
    this->ui->DOMICILIO_LE->setText(selected_list->client_info.DOMICILIO);
    this->ui->CIUDAD_LE->setText(selected_list->client_info.CIUDAD);

    this->ui->RFC_LE->setText(selected_list->client_info.RFC);
    this->ui->AGENTE_LE->setText(selected_list->client_info.AGENTE);
    this->ui->CONDICIONES_LE->setText(selected_list->client_info.CONDICIONES);
    this->ui->DISCOUNT_LE->setText(QString::number(selected_list->client_info.DISCOUNT));

    // show the models of this entry
    for(EntryPtr e : selected_list->entryList.entries){
        list_models_table->insertRow(list_models_table->rowCount());

        QVector<QString> items = e->view_values();

        for(UI col = 0; col < items.size(); col++){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );
            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            list_models_table->setItem(list_models_table->rowCount()-1, col, tableWidgetItem);
        }
    }
}


void Search_List_Win::reset_list_info()
{
    this->selected_list = nullptr;

    list_models_table->clearContents();
    list_models_table->setRowCount(0);

    this->ui->CLIENTE_LE->clear();
    this->ui->DOMICILIO_LE->clear();
    this->ui->CIUDAD_LE->clear();
    this->ui->RFC_LE->clear();
    this->ui->AGENTE_LE->clear();
    this->ui->CONDICIONES_LE->clear();
    this->ui->DISCOUNT_LE->clear();
}


// set language
void Search_List_Win::setLanguage()
{
    this->ui->list_id_2be_searched_label->setText(lan("清单号", "número de lista"));

    this->ui->list_id_2be_searched_LE->setPlaceholderText(lan("在此输入需要查询的清单号", "Ingrese el número de lista que desea consultar aquí"));

    this->ui->search_list_result_GB->setTitle(lan("清单查询结果", "resultados de la consulta de la lista"));

    QStringList headers = {
       lan("清单号", "número de lista"),
       lan("创建日期", "fecha de creación"),
       lan("创建时间", "hora de creación"),
       lan("总箱数", "número total de cajas"),
       lan("价格", "Piezas por caja"),
       lan("客户", "número de contenedor")
    };
    this->ui->searched_lists_table->setHorizontalHeaderLabels(headers);

    this->ui->selected_list_GB->setTitle(lan("选中清单的详细信息", "detalles de la lista seleccionada"));

    this->ui->selected_list_info_GB->setTitle(lan("清单信息", "información de la lista"));

    const QString enter_here = lan("在此输入", "ingresa aquí");

    this->ui->CLIENTE_label->setText(lan("客户", "CLIENTE"));
    this->ui->CLIENTE_LE->setPlaceholderText(enter_here);

    this->ui->DOMICILIO_label->setText(lan("地址", "DOMICILIO"));
    this->ui->DOMICILIO_LE->setPlaceholderText(enter_here);

    this->ui->CIUDAD_label->setText(lan("城市", "CIUDAD"));
    this->ui->CIUDAD_LE->setPlaceholderText(enter_here);

    this->ui->RFC_label->setText("R.F.C");
    this->ui->RFC_LE->setPlaceholderText(enter_here);

    this->ui->AGENTE_label->setText(lan("代理", "AGENTE"));
    this->ui->AGENTE_LE->setPlaceholderText(enter_here);

    this->ui->CONDICIONES_label->setText(lan("付款方式", "CONDICIONES DE PAGO"));
    this->ui->CONDICIONES_LE->setPlaceholderText(enter_here);

    this->ui->DISCOUNT_label->setText(lan("折扣", "DISCOUNT(%)"));
    this->ui->DISCOUNT_LE->setPlaceholderText(enter_here);

    this->ui->list_items_GB->setTitle(lan("清单里的货物", "productos en la lista"));

    this->ui->delete_list_btn->setText(lan("删除此清单（不返还库存）", "eliminar esta lista (sin devolver el inventario)"));

    this->ui->put_back_list_btn->setText(lan("退清单（返还库存）", "devolver lista (devolver inventario)"));
}


void Search_List_Win::closeEvent (QCloseEvent *event)
{
    QMessageBox msg(this);
    msg.setText(lan(Are_You_Sure_to_Exit_CN, Are_You_Sure_to_Exit_SPAN));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::No) {
        event->ignore();
    }
    else {
        if(this->parentWin != nullptr) {
            // since models can be modified, we need to update the parentWindow GUI as well
            this->parentWin->update_GUI();
            this->parentWin->show();
        }
        event->accept();
    }
}


// user is typing the list id, and we use it to search lists
void Search_List_Win::on_list_id_2be_searched_LE_textChanged(const QString & list_id_prefix)
{
    // disable the window in case search takes time
    this->setDisabled(true);

    QString userInput = list_id_prefix.trimmed(); // remove useless empty spaces

    // clear content of <searched_lists_table>
    searched_lists_table->clearContents();
    searched_lists_table->setRowCount(0);

    QVector<ListPtr> candidates;
    lists.get_list(userInput, candidates, true);

    // for each list, make a row for it
    for(UI row = 0; row < candidates.size(); row++){
        const ListPtr list = candidates[row];

        searched_lists_table->insertRow(searched_lists_table->rowCount());

        QVector<QString> items = list->describe_this_list();

        for( UI col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            searched_lists_table->setItem(row, col, tableWidgetItem);
        }
    }

    // clear the selected list
    this->reset_list_info();

    this->setEnabled(true);
    this->ui->list_id_2be_searched_LE->setFocus();
}


void Search_List_Win::on_searched_lists_table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    this->setDisabled(true);

    // clear content of <list_models_table>
    list_models_table->clearContents();
    list_models_table->setRowCount(0);

    searched_lists_table->selectRow(row);
    QList items = searched_lists_table->selectedItems();
    QString list_id = items[searched_lists_table_list_id_idx]->text();

    unsigned long int list_ID = list_id.toULong();

    ListPtr list = lists.get_list(list_ID);
    if(list.isNull()){
        goto Finish;
    }

    this->selected_list = list;

    // put the information of this list on UI
    this->view_selected_list();

Finish:
    this->setEnabled(true);
}


// delete the selected list
void Search_List_Win::on_delete_list_btn_clicked()
{
    this->setDisabled(true);

    QMessageBox msg;
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    // display message if nothing is selected
    if(this->selected_list.isNull()){
        msg.setText("没有选中任何清单！");
        msg.exec();
        goto Finish;
    }

    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setText("确定要删除单号为" + QString::number(this->selected_list->id) + "的清单吗？");

    if (msg.exec() == QMessageBox::Yes) {
        // 删除该清单
        lists.remove_list(this->selected_list->id);
        // 更新GUI
        this->on_list_id_2be_searched_LE_textChanged(this->ui->list_id_2be_searched_LE->text());

        //更新 lists存储文件
        lists.save_2_file();

        goto Finish;
    }
Finish:
    this->selected_list = nullptr;

    // save the inventory and lists
    WriteFile wf;
    wf.Inventory2Xlsx();
    wf.Lists2txt(Lists_FNAME);

    this->setEnabled(true);
}


// put back the list, put back all its items
void Search_List_Win::on_put_back_list_btn_clicked()
{
    QMessageBox msg;
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    // display message if nothing is selected
    if(this->selected_list.isNull()){
        msg.setText("没有选中任何清单！");
        msg.exec();
        goto Finish;
    }

    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setText("确定要退单号为" + QString::number(this->selected_list->id) + "的清单吗？");

    if (msg.exec() == QMessageBox::Yes) {
        // 将清单内的货物加回库存
        this->selected_list->return_models();

        // 删除该清单
        lists.remove_list(this->selected_list->id);
        // 更新GUI
        this->on_list_id_2be_searched_LE_textChanged(this->ui->list_id_2be_searched_LE->text());

        //更新 lists存储文件
        lists.save_2_file();

        goto Finish;
    }

Finish:
    // save the inventory and lists
    WriteFile::Inventory2Xlsx();
    WriteFile::Lists2txt(Lists_FNAME);

    this->selected_list = nullptr;
}

