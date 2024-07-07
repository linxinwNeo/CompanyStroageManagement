#include "Search_List_Win.h"
#include "LanguageStrings.h"
#include "FileLoader/WriteFile.h"
#include "GlobalVars.h"
#include "ui_Search_List_Win.h"

#include <QMessageBox>
#include <QCloseEvent>
#include "mainwindow.h"

Search_List_Win::Search_List_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search_List_Win)
{
    ui->setupUi(this);
    
    search_result_Table = ui->search_result_Table;
    //search_result_Table->setStyleSheet(table_stylesheet);

    selected_list_entries_Table = ui->selected_list_entries_Table;
    //selected_list_entries_Table->setStyleSheet(table_stylesheet);

    this->on_list_id_2be_searched_LE_textChanged( this->ui->list_id_2be_searched_LE->text() );

    this->setWindow();

    this->set_GUI_Language();
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
    for(EntryPtr& e : selected_list->entryList.entries){
        selected_list_entries_Table->insertRow(selected_list_entries_Table->rowCount());

        QVector<QString> items = e->view_values();

        for(unsigned long col = 0; col < items.size(); col++){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );
            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            selected_list_entries_Table->setItem(selected_list_entries_Table->rowCount()-1, col, tableWidgetItem);
        }
    }
}


void Search_List_Win::reset_selected_list_info()
{
    this->selected_list = nullptr;

    selected_list_entries_Table->clearContents();
    selected_list_entries_Table->setRowCount(0);

    this->ui->CLIENTE_LE->clear();
    this->ui->DOMICILIO_LE->clear();
    this->ui->CIUDAD_LE->clear();
    this->ui->RFC_LE->clear();
    this->ui->AGENTE_LE->clear();
    this->ui->CONDICIONES_LE->clear();
    this->ui->DISCOUNT_LE->clear();
}


// 更新 search_result_Table
void Search_List_Win::update_search_result_Table()
{
    this->ui->list_id_2be_searched_LE->clear();
    this->on_list_id_2be_searched_LE_textChanged(this->ui->list_id_2be_searched_LE->text());
}


// set language
void Search_List_Win::set_GUI_Language()
{
    this->ui->list_id_2be_searched_label->setText(lan("清单号", "número de lista"));

    this->ui->list_id_2be_searched_LE->setPlaceholderText(lan("在此输入需要查询的清单号", "Ingrese el número de lista que desea consultar aquí"));

    this->ui->search_list_result_GB->setTitle(lan("清单查询结果", "resultados de la consulta de la lista"));

    QStringList headers = {
       lan("清单号", "Número de lista"),
       lan("创建时间", "Tiempo de creación"),
       lan("总箱数", "Número total de cajas"),
       lan("总价", "IMPORTE"),
       lan("客户", "CLIENTE")
    };
    search_result_Table->setHorizontalHeaderLabels(headers);

    this->ui->selected_list_GB->setTitle(lan("选中清单的详细信息", "detalles de la lista seleccionada"));

    this->ui->selected_list_info_GB->setTitle(lan("清单信息", "información de la lista"));

    const QString none = lan("暂无", "ingresa aquí");

    this->ui->CLIENTE_label->setText(lan("客户", "CLIENTE"));
    this->ui->CLIENTE_LE->setPlaceholderText(none);

    this->ui->DOMICILIO_label->setText(lan("地址", "DOMICILIO"));
    this->ui->DOMICILIO_LE->setPlaceholderText(none);

    this->ui->CIUDAD_label->setText(lan("城市", "CIUDAD"));
    this->ui->CIUDAD_LE->setPlaceholderText(none);

    this->ui->RFC_label->setText("R.F.C");
    this->ui->RFC_LE->setPlaceholderText(none);

    this->ui->AGENTE_label->setText(lan("代理", "AGENTE"));
    this->ui->AGENTE_LE->setPlaceholderText(none);

    this->ui->CONDICIONES_label->setText(lan("付款方式", "CONDICIONES DE PAGO"));
    this->ui->CONDICIONES_LE->setPlaceholderText(none);

    this->ui->DISCOUNT_label->setText(lan("折扣", "DISCOUNT(%)"));
    this->ui->DISCOUNT_LE->setPlaceholderText(none);

    this->ui->list_items_GB->setTitle(lan("清单里的货物", "Mercancías en la lista"));

    this->ui->delete_list_btn->setText(lan("删除此清单（不退还库存）", "Eliminar esta lista (sin devolver al inventario)"));

    this->ui->put_back_list_btn->setText(lan("退回清单（返还库存）", "Devolver la lista (reintegrar al inventario)"));


    // 设置 selected_list_entries_Table 的语言
    QStringList headers2 = {
        lan("货号", "CLAVE"),
        lan("集装箱号", "CONTAINER"),
        lan("品名(中文)", "Nombre del producto (en chino)"),
        lan("品名(西语)", "Nombre del producto (en español)"),
        lan("箱数", "CAJA"),
        lan("每箱个数", "CANT POR CAJA"),
        lan("个数", "CAMTODAD"),
        lan("单价", "PRECOP U."),
        lan("总价", "IMPORTE")
    };
    selected_list_entries_Table->setHorizontalHeaderLabels(headers2);
}


void Search_List_Win::setWindow()
{
    QScreen *screen = QApplication::screens().at(0);
    QRect screenSize = screen->availableGeometry();

    int width = static_cast<int>(screenSize.width() * widthRatio);
    int height = static_cast<int>(screenSize.height() * heightRatio);

    this->resize(width, height);

    this->move(screenSize.width() / 2. - width / 2., screenSize.height() / 2. - height / 2.);
}


void Search_List_Win::closeEvent (QCloseEvent *event)
{
    QMessageBox msg(this);
    msg.setText(lan("你确定要返回主界面吗？", "¿Seguro que quieres volver a la pantalla principal?"));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::No) {
        event->ignore();
    }
    else {
        if(this->parentWin != nullptr) {
            // since models can be modified, we need to update the parentWindow GUI as well
            this->parentWin->update_GUI();
            this->parentWin->show();

            // we want to clear up the content we currently have in this window
            this->reset_selected_list_info();
            this->ui->list_id_2be_searched_LE->clear();
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
    search_result_Table->clearContents();
    search_result_Table->setRowCount(0);

    QVector<ListPtr> candidates;
    lists.get_list(userInput, candidates, true);

    // for each list, make a row for it
    for( unsigned long row = 0; row < candidates.size(); row++ ){
        const ListPtr list = candidates[row];

        search_result_Table->insertRow(search_result_Table->rowCount());

        QVector<QString> items = list->describe_this_list();

        for( unsigned long col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            search_result_Table->setItem(row, col, tableWidgetItem);
        }
    }

    // clear the selected list
    this->reset_selected_list_info();

    this->setEnabled(true);
    this->ui->list_id_2be_searched_LE->setFocus();
}


void Search_List_Win::on_search_result_Table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    this->setDisabled(true);

    // clear content of <selected_list_entries_Table>
    selected_list_entries_Table->clearContents();
    selected_list_entries_Table->setRowCount(0);

    search_result_Table->selectRow(row);
    QList items = search_result_Table->selectedItems();
    QString list_id = items[searched_lists_table_list_id_idx]->text();

    const unsigned long list_ID = list_id.toULong();

    ListPtr list = lists.get_list(list_ID);
    if(list.isNull()){ // no such list exists...but this should not happen
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
    QMessageBox msg;
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    // display message if nothing is selected
    if(this->selected_list.isNull()){
        msg.setText(lan("没有选中任何清单！", "No se ha comprobado ningún listado."));
        msg.exec();
        goto Finish;
    }

    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setText(lan("确定要删除单号为", "¿Estás seguro de que quieres borrar la lista con el número de pedido") +
                QString::number(this->selected_list->id) + lan("的清单吗？", "?"));

    if (msg.exec() == QMessageBox::Yes) {
        // 删除该清单
        lists.remove_list(this->selected_list->id);
        // 更新GUI
        this->on_list_id_2be_searched_LE_textChanged(this->ui->list_id_2be_searched_LE->text());

        // 更新 lists存储文件
        lists.save_2_file(false);

        goto Finish;
    }
Finish:
    this->selected_list = nullptr;

    // save the inventory and lists
    WriteFile::SaveInventoryAuto(false);
    WriteFile::Lists2txt(false);
}


// put back the list, put back all its items
void Search_List_Win::on_put_back_list_btn_clicked()
{
    QMessageBox msg;
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // display message if nothing is selected
    if(this->selected_list.isNull()){
        msg.setText(lan("没有选中任何清单！", "No se ha comprobado ningún listado."));
        msg.exec();
        goto Finish;
    }

    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setText(lan("确定要退单号为", "¿Estás seguro de que quieres devolver la lista con el número de pedido") +
                QString::number(this->selected_list->id) + lan("的清单吗？", "?"));

    if (msg.exec() == QMessageBox::Yes) {
        // 将清单内的货物加回库存
        this->selected_list->AddBack_Models();

        // 删除该清单
        lists.remove_list(this->selected_list->id);
        // 更新GUI
        this->on_list_id_2be_searched_LE_textChanged(this->ui->list_id_2be_searched_LE->text());

        //更新 lists存储文件
        lists.save_2_file(false);

        // save the inventory and lists
        WriteFile::SaveInventoryAuto(false);

        QMessageBox msg2;
        msg2.setText(lan("退订单成功！", "¡El pedido se ha devuelto con éxito!"));
        msg2.exec();

        goto Finish;
    }
    else{
        return;
    }

Finish:
    this->selected_list = nullptr;
}

