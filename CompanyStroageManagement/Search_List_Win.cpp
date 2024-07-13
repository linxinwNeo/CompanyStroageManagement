#include "Search_List_Win.h"
#include "ui_Search_List_Win.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStandardPaths>

#include "mainwindow.h"
#include "FileLoader/WriteFile.h"
#include "GlobalVars.h"
#include "LanguageStrings.h"
#include "Others/create_pdf.h"

Search_List_Win::Search_List_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search_List_Win)
{
    ui->setupUi(this);
    
    search_result_Table = ui->search_result_Table;

    selected_list_entries_Table = ui->selected_list_entries_Table;

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

    this->ui->lineEdit_ClientID->setText(selected_list->client_info.m_ID);
    this->ui->lineEdit_CLIENTE->setText(selected_list->client_info.m_clientName);
    this->ui->lineEdit_DOMICILIO->setText(selected_list->client_info.m_DOMICILIO);
    this->ui->lineEdit_CIUDAD->setText(selected_list->client_info.m_CIUDAD);

    this->ui->lineEdit_RFC->setText(selected_list->client_info.m_RFC);
    this->ui->lineEdit_AGENTE->setText(selected_list->client_info.m_AGENTE);
    this->ui->lineEdit_CONDICIONES->setText(selected_list->client_info.m_CONDICIONES);
    this->ui->lineEdit_DISCOUNT->setText(QString::number(selected_list->client_info.m_DISCOUNT));

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

    this->ui->lineEdit_ClientID->clear();
    this->ui->lineEdit_CLIENTE->clear();
    this->ui->lineEdit_DOMICILIO->clear();
    this->ui->lineEdit_CIUDAD->clear();
    this->ui->lineEdit_RFC->clear();
    this->ui->lineEdit_AGENTE->clear();
    this->ui->lineEdit_CONDICIONES->clear();
    this->ui->lineEdit_DISCOUNT->clear();
}


// 更新 search_result_Table
void Search_List_Win::clear_tables()
{
    this->ui->lineEdit_list_id_2be_searched->clear();
    this->ui->lineEdit_ClientIDPrefix->clear();
    this->ui->lineEdit_ClientNamePrefix->clear();

    this->search_result_Table->clearContents();
    this->search_result_Table->setRowCount(0);

    this->selected_list = nullptr;
    this->selected_list_entries_Table->clearContents();
    this->selected_list_entries_Table->setRowCount(0);
}


// set language
void Search_List_Win::set_GUI_Language()
{
    this->ui->label_list_id_2be_searched->setText(lan("清单号", "número de lista"));

    this->ui->lineEdit_list_id_2be_searched->setPlaceholderText(lan("在此输入需要查询的清单号", "Ingrese el número de lista que desea consultar aquí"));

    this->ui->search_list_result_GB->setTitle(lan("清单查询结果", "resultados de la consulta de la lista"));

    QStringList headers = {
       lan("清单号", "Número de lista"),
       lan("创建时间", "Tiempo de creación"),
       lan("总箱数", "Número total de cajas"),
       lan("总价($)", "IMPORTE($)"),
       lan("客户", "CLIENTE")
    };
    search_result_Table->setHorizontalHeaderLabels(headers);

    this->ui->selected_list_GB->setTitle(lan("选中清单的详细信息", "detalles de la lista seleccionada"));

    this->ui->selected_list_info_GB->setTitle(lan("清单信息", "información de la lista"));

    const QString none = lan("暂无", "ingresa aquí");

    this->ui->label_ClientID->setText(lan("客户号", "Número de cliente"));
    this->ui->lineEdit_ClientID->setPlaceholderText(none);

    this->ui->label_CLIENTE->setText(lan("客户", "CLIENTE"));
    this->ui->lineEdit_CLIENTE->setPlaceholderText(none);

    this->ui->label_DOMICILIO->setText(lan("地址", "DOMICILIO"));
    this->ui->lineEdit_DOMICILIO->setPlaceholderText(none);

    this->ui->label_CIUDAD->setText(lan("城市", "CIUDAD"));
    this->ui->lineEdit_CIUDAD->setPlaceholderText(none);

    this->ui->label_RFC->setText("R.F.C");
    this->ui->lineEdit_RFC->setPlaceholderText(none);

    this->ui->label_AGENTE->setText(lan("代理", "AGENTE"));
    this->ui->lineEdit_AGENTE->setPlaceholderText(none);

    this->ui->label_CONDICIONES->setText(lan("付款方式", "CONDICIONES DE PAGO"));
    this->ui->lineEdit_CONDICIONES->setPlaceholderText(none);

    this->ui->label_DISCOUNT->setText(lan("折扣(%)", "DISCOUNT(%)"));
    this->ui->lineEdit_DISCOUNT->setPlaceholderText(none);

    this->ui->list_items_GB->setTitle(lan("清单里的货物", "Mercancías en la lista"));

    this->ui->pushButton_delete_list->setText(lan("删除此清单（不退还库存）", "Eliminar esta lista (sin devolver al inventario)"));

    this->ui->pushButton_put_back_list->setText(lan("退回清单（返还库存）", "Devolver la lista (reintegrar al inventario)"));


    // 设置 selected_list_entries_Table 的语言
    QStringList headers2 = {
        lan("货号", "CLAVE"),
        lan("集装箱号", "CONTAINER"),
        lan("品名(中文)", "Nombre del producto (en chino)"),
        lan("品名(西语)", "Nombre del producto (en español)"),
        lan("箱数", "CAJA"),
        lan("每箱个数", "CANT POR CAJA"),
        lan("个数", "CAMTODAD"),
        lan("单价($)", "PRECOP U.($)"),
        lan("总价($)", "IMPORTE($)")
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
            this->ui->lineEdit_list_id_2be_searched->clear();
        }
        event->accept();
    }
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

    ListPtr list = listManager.get_list(list_ID);
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
void Search_List_Win::on_pushButton_delete_list_clicked()
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
        listManager.delete_list(this->selected_list->id);
        // 更新GUI
        this->clear_tables();

        goto Finish;
    }
Finish:
    this->selected_list = nullptr;

    // save the inventory and lists
    WriteFile::SaveInventoryAuto(false);
}


// put back the list, put back all its items
void Search_List_Win::on_pushButton_put_back_list_clicked()
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
        listManager.delete_list(this->selected_list->id);
        // 更新GUI
        this->clear_tables();

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


// 生成一个pdf文件
void Search_List_Win::on_pushButton_createPDF_clicked()
{
    if(this->selected_list.isNull()){
        QMessageBox msg;
        msg.setText(lan("必须先选中一个清单！", "¡Primero tienes que revisar una lista!"));
        msg.exec();
        return;
    }

    QString filePath;
    QMessageBox Msgbox;
    QFileDialog saveFileDialog;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);



    // ask for the path to store the file
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    saveFileDialog.setDirectory(desktopPath);
    saveFileDialog.setWindowTitle(lan(WHERE_TO_SAVE_FILE_MESSAGE_CN, WHERE_TO_SAVE_FILE_MESSAGE_SPAN));
    saveFileDialog.setDefaultSuffix("pdf");
    saveFileDialog.setNameFilter("PDF (*.pdf)");
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);

    if (saveFileDialog.exec()) {
        // Do something with the selected file
        filePath = saveFileDialog.selectedFiles().at(0).trimmed();
    }

    if(filePath.isEmpty()) goto Fail;

    // create PDF file
    create_pdf(filePath, this->selected_list);

    goto Success;


Success:
    // display creation success
    Msgbox.setText(lan("PDF创建成功！", "¡El PDF ha sido creado con éxito!"));
    Msgbox.exec();
    return;

Fail:
    return;
}


void Search_List_Win::on_pushButton_SearchListsByListIDPrefix_clicked()
{
    // disable the window in case search takes time
    this->setDisabled(true);

    this->ui->lineEdit_ClientIDPrefix->clear();
    this->ui->lineEdit_ClientNamePrefix->clear();

    QString userInput = this->ui->lineEdit_list_id_2be_searched->text().trimmed(); // remove useless empty spaces


    // clear content of <searched_lists_table>
    this->clear_tables();

    QVector<ListPtr> candidates;
    listManager.get_lists(userInput, candidates, true);

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
    this->ui->lineEdit_list_id_2be_searched->setFocus();
}


void Search_List_Win::on_pushButton_SearchListsByClientIDPrefix_clicked()
{

}


void Search_List_Win::on_pushButton_SearchListsByClientNamePrefix_clicked()
{

}

