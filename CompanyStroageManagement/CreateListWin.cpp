#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextEdit>
#include <QStandardPaths>

#include "LanguageStrings.h"
#include "CreateListWin.h"
#include "FileLoader/WriteFile.h"
#include "Others/create_PDF.h"
#include "mainwindow.h"
#include "ui_CreateListWin.h"
#include "GlobalVars.h"
#include "Others/write_error_file.h"

EntryList cur_list_entries;

CreateListWin::CreateListWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateListWin)
{
    ui->setupUi(this);

    this->init();
    this->set_GUI_Language();

    this->setWindow();

    this->setWindowTitle(lan("新清单", "Nueva lista"));
}


CreateListWin::~CreateListWin()
{
    delete ui;

    cur_list_entries.clear_memory();
}


void CreateListWin::setWindow()
{
    QScreen *screen = QApplication::screens().at(0);
    QRect screenSize = screen->availableGeometry();

    int width = static_cast<int>(screenSize.width() * widthRatio);
    int height = static_cast<int>(screenSize.height() * heightRatio);

    this->resize(width, height);

    this->move(screenSize.width() / 2 - width / 2, screenSize.height() / 2 - height / 2);
}


// update_added_models_table_according to the cur_list_entries
void CreateListWin::update_added_models_table()
{
    this->setDisabled(true);

    this->clear_added_models_table();

    // for each entry, make a row for it
    for(unsigned long row = 0; row < cur_list_entries.num_entries(); row++){
        const EntryPtr entry = cur_list_entries.get_entry(row);
        if(entry.isNull()) continue;

        added_models_table->insertRow(added_models_table->rowCount());

        auto rowValues = entry->view_values();

        for( unsigned long col = 0; col < rowValues.size(); col++ ){
            if (col == 2 || col == 3) { // Replace with QTextEdit when col reaches 2 or 3
                QTextEdit *textEdit = new QTextEdit();
                textEdit->setPlainText(rowValues[col]);
                textEdit->setReadOnly(true); // Optionally make it read-only

                // Set QTextEdit as the cell widget
                added_models_table->setCellWidget(row, col, textEdit);
            } else {
                QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
                tableWidgetItem->setText( rowValues[col] );

                tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

                added_models_table->setItem(row, col, tableWidgetItem);
            }
        }
    }

//Finish:
    this->setEnabled(true);
}


void CreateListWin::remove_entry(const unsigned int idx)
{
    cur_list_entries.remove_entry(idx);
    this->update_added_models_table();
}


// set language
void CreateListWin::set_GUI_Language()
{
    this->ui->client_info_GB->setTitle(lan("清单信息", "información de la lista"));

    const QString enter_here = lan("在此输入", "ingresa aquí");

    this->ui->label_CLIENTE->setText(lan("客户名", "CLIENTE"));
    this->ui->lineEdit_CLIENTE->setPlaceholderText(enter_here);

    this->ui->label_clientID->setText(lan("客户号码", "Número de cliente"));
    this->ui->lineEdit_clientID->setPlaceholderText(enter_here);

    this->ui->label_DOMICILIO->setText(lan("地址", "DOMICILIO"));
    this->ui->lineEdit_DOMICILIO->setPlaceholderText(enter_here);

    this->ui->label_CIUDAD->setText(lan("城市", "CIUDAD"));
    this->ui->lineEdit_CIUDAD->setPlaceholderText(enter_here);

    this->ui->label_RFC->setText("R.F.C.");
    this->ui->lineEdit_RFC->setPlaceholderText(enter_here);

    this->ui->label_AGENTE->setText(lan("代理", "AGENTE"));
    this->ui->lineEdit_AGENTE->setPlaceholderText(enter_here);

    this->ui->label_CONDICIONES->setText(lan("付款方式", "CONDICIONES DE PAGO"));
    this->ui->lineEdit_CONDICIONES->setPlaceholderText(enter_here);

    this->ui->label_discount->setText(lan("折扣(%)", "DISCOUNT(%)"));

    this->ui->reset_client_info_btn->setText(lan("重置信息", "resetear la información"));

    this->ui->searchModel_TW->setTabText(0, lan("使用货号搜索需要添加的货物", "buscar los productos a agregar utilizando el número de referencia"));

    this->ui->model_code_for_search_label->setText(lan("货号", "MODELO"));
    this->ui->model_code_for_search_LE->setPlaceholderText(lan("在此输入需要搜索的货号 比如 TA-0001", "Introduce el número de referencia que deseas buscar aquí, por ejemplo, TA-0001"));

    this->ui->searched_models_table->setHorizontalHeaderLabels(GlobalVars::table_headers_model());

    this->ui->add_selected_model_btn->setText(lan("添加选中的货物", "añadir los productos seleccionados"));

    this->ui->added_models_GB->setTitle(lan("已添加的货物（双击货物修改数量）", "Productos añadidos (doble clic para modificar la cantidad)"));

    this->ui->remove_selected_model_btn->setText(lan("删除选中的货物", "eliminar los productos seleccionados"));

    this->ui->reset_added_models_table_btn->setText(lan("删除所有已添加的货物", "eliminar todos los productos añadidos"));

    this->ui->generate_list_GB->setTitle(lan("生成清单", "crear lista"));

    this->ui->previewList_btn->setText(lan("生成预览清单", "crear una vista previa de la lista"));

    this->ui->generatePDF_btn->setText(lan("生成正式清单", "Genere una lista de verificación formal"));

    this->ui->pushButton_edit_selected_model->setText(lan("修改选中的货物", "Modificar los productos seleccionados"));

    this->ui->pushButton_searchModel->setText(lan("搜索", "Buscar"));
}


// create the pdf and deduct all listed items from the stroage
// also clear the <added_models_table>
void CreateListWin::on_generatePDF_btn_clicked()
{
    QString filePath;
    QMessageBox Msgbox;
    QFileDialog saveFileDialog;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QSharedPointer<Client> new_client = nullptr;

    if(this->ui->lineEdit_clientID->text().trimmed() == "")
    {
        Msgbox.setText(lan("客户号码不能为空！", "¡El número de cliente no puede estar vacío!"));
        Msgbox.exec();
        return;
    }

    if(this->ui->lineEdit_CLIENTE->text().trimmed() == "")
    {
        Msgbox.setText(lan("客户名不能为空！", "¡El nombre del cliente no puede estar vacío!"));
        Msgbox.exec();
        return;
    }

    if(cur_list_entries.num_entries() == 0) {
        Msgbox.setText(lan("清单是空的！", "¡La lista está vacía!"));
        Msgbox.exec();
        return;
    }

    const unsigned long int unused_unique_id = listManager.get_unique_id();

    QString PDF_MSG_1 = lan(PDF_MESSAGE_1_CN, PDF_MESSAGE_1_SPAN);
    QString PDF_MSG_2 = lan(PDF_MESSAGE_2_CN, PDF_MESSAGE_2_SPAN);

    QMessageBox::StandardButton reply = QMessageBox::question(this, PDF_MSG_1, PDF_MSG_2,
                                                              QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) goto Fail; // return if the user says no

    this->list = ListPtr(new List());
    
    this->list->entryList = cur_list_entries;

    // save client info
    this->list->client_info.m_ID = this->ui->lineEdit_clientID->text();
    this->list->client_info.m_clientName = this->ui->lineEdit_CLIENTE->text();
    this->list->client_info.m_DOMICILIO = this->ui->lineEdit_DOMICILIO->text();
    this->list->client_info.m_CIUDAD = this->ui->lineEdit_CIUDAD->text();
    this->list->client_info.m_RFC = this->ui->lineEdit_RFC->text();
    this->list->client_info.m_AGENTE = this->ui->lineEdit_AGENTE->text();
    this->list->client_info.m_CONDICIONES = this->ui->lineEdit_CONDICIONES->text();
    this->list->client_info.m_DISCOUNT = this->ui->doubleSpinBox_discount->value() / 100.; // the value the user is entering is between 0-100
    this->list->client_info.m_TOTAL_NUM_BOXES = this->list->total_num_boxes();
    this->list->id = unused_unique_id;

    this->list->datetime_created = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());

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
    create_pdf(filePath, this->list);

    // now deduct the items from stroage
    inventory.deduct_models(this->list->entryList.entries);

    // save the list to a txt file
    listManager.create_list(this->list);


    cur_list_entries.clear_memory();

    this->clear_added_models_table();

    this->on_pushButton_searchModel_clicked();
    this->selected_model_in_search_table = nullptr; // reset selected model

    goto Success;

Success:
    // save the inventory and lists
    WriteFile::SaveInventoryAuto(false);

    // display creation success
    Msgbox.setText(lan(LIST_CREATED_MSG_CN, LIST_CREATED_MSG_SPAN));
    Msgbox.exec();

    // save client info
    new_client = QSharedPointer<Client>::create( Client(
        this->ui->lineEdit_clientID->text().trimmed(),
        this->ui->lineEdit_CLIENTE->text().trimmed(),
        this->ui->lineEdit_DOMICILIO->text().trimmed(),
        this->ui->lineEdit_CIUDAD->text().trimmed(),
        this->ui->lineEdit_RFC->text().trimmed(),
        this->ui->lineEdit_AGENTE->text().trimmed(),
        this->ui->lineEdit_CONDICIONES->text().trimmed()
        ) );
    clientManager.add_client(new_client);

    // reset client information
    this->clear_client_info();

    return;

Fail:
    return;
}


void CreateListWin::clear_client_info()
{
    this->ui->lineEdit_clientID->setText("");
    this->ui->lineEdit_CLIENTE->setText("");
    this->ui->lineEdit_DOMICILIO->setText("");
    this->ui->lineEdit_CIUDAD->setText("");
    this->ui->lineEdit_RFC->setText("");
    this->ui->lineEdit_AGENTE->setText("");
    this->ui->lineEdit_CONDICIONES->setText("");
    this->ui->doubleSpinBox_discount->setValue(0.0);
}

// create a pdf but do not deduct items from the stroage
void CreateListWin::on_previewList_btn_clicked()
{
    QString filePath;
    QMessageBox Msgbox;
    QFileDialog saveFileDialog;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    if(cur_list_entries.num_entries() == 0) {
        Msgbox.setText(lan(EMPTY_LIST_CN, EMPTY_LIST_SPAN));
        Msgbox.exec();
        return;
    }

    QString PDF_MSG_1 = lan(PDF_MESSAGE_1_CN, PDF_MESSAGE_1_SPAN);
    QString PDF_MSG_2 = lan(PDF_MESSAGE_2_CN, PDF_MESSAGE_2_SPAN);
    QMessageBox::StandardButton reply = QMessageBox::question(this, PDF_MSG_1, PDF_MSG_2,
                                                            QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) goto Finish;

    this->list = ListPtr(new List());
    
    this->list->entryList = cur_list_entries;

    // save client info
    this->list->client_info.m_clientName = this->ui->lineEdit_CLIENTE->text();
    this->list->client_info.m_DOMICILIO = this->ui->lineEdit_DOMICILIO->text();
    this->list->client_info.m_CIUDAD = this->ui->lineEdit_CIUDAD->text();
    this->list->client_info.m_RFC = this->ui->lineEdit_RFC->text();
    this->list->client_info.m_AGENTE = this->ui->lineEdit_AGENTE->text();
    this->list->client_info.m_CONDICIONES = this->ui->lineEdit_CONDICIONES->text();
    this->list->client_info.m_DISCOUNT = this->ui->doubleSpinBox_discount->value() / 100.; // the value the user is entering is between 0-100
    this->list->client_info.m_TOTAL_NUM_BOXES = this->list->total_num_boxes();
    this->list->id = 0; // set id to 0 for preview lists
    this->list->datetime_created = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());


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


    if(filePath.isEmpty()) goto Finish;

    // create PDF file
    create_pdf(filePath, this->list);


    // display creation success
    Msgbox.setText(lan(LIST_CREATED_MSG_CN, LIST_CREATED_MSG_SPAN));
    Msgbox.exec();

Finish:
    return;
}


void CreateListWin::closeEvent (QCloseEvent *event)
{
    QMessageBox msg;
    msg.setText(lan("你确定要返回主界面吗？", "¿Seguro que quieres volver a la pantalla principal?"));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    int resBtn = msg.exec();
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    }
    else {
        this->parentPtr->show();
        event->accept();
    }
}


// 初始化
void CreateListWin::init()
{
    //初始化变量
    this->searched_models_table = this->ui->searched_models_table;
    this->added_models_table = this->ui->added_models_table;
    this->selected_model_in_search_table = nullptr;
    this->list = nullptr;
    this->adjust_list_item_win = nullptr;
}


// 清理表格数据
void CreateListWin::clear_searched_models_table()
{
    if(!this->searched_models_table) return;
    searched_models_table->clearContents(); // clear the table contents but columns are reserved
    searched_models_table->setRowCount(0);

    this->selected_model_in_search_table = nullptr;
}


// 清理表格数据
void CreateListWin::clear_added_models_table()
{
    if(!this->added_models_table) return;
    added_models_table->clearContents(); // clear the table contents but columns are reserved
    added_models_table->setRowCount(0);
}


/* try to add selected model in to the <added_models_table>, but we need to check if it has been added already */
void CreateListWin::on_add_selected_model_btn_clicked()
{
    if(this->selected_model_in_search_table.isNull()) return;

    // we dont have this selected model anymore
    QMessageBox Msgbox;

    if(this->selected_model_in_search_table->NUM_LEFT_PIECES() == 0){
        Msgbox.setText(lan(OUT_OF_STOCK_MSG_CN, OUT_OF_STOCK_MSG_SPAN));
        Msgbox.exec();
        return;
    }

    ModelPtr model_2be_added;
    QString MODELCODE_2be_Added;
    QString ContainerID_2be_Added;

    unsigned long NUM_LEFT_PIECES = 0.;
    unsigned long NUM_PIECES_PER_BOX = 0.;
    QString DESCRIPTION_SPAN;
    QString DESCRIPTION_CN;
    double PRIZE = 0.;
    EntryPtr new_entry;

    this->setDisabled(true);

    model_2be_added = this->selected_model_in_search_table;
    MODELCODE_2be_Added = model_2be_added->m_MODEL_CODE;

    if(this->selected_model_in_search_table->m_Container.isNull()) ContainerID_2be_Added.clear();
    else ContainerID_2be_Added = model_2be_added->m_Container->ID;

    // check if this model has been added to <selected_list_entries_Table> already
    if(cur_list_entries.has_Model(MODELCODE_2be_Added, ContainerID_2be_Added)){
        Msgbox.setText(lan(MODEL_EXIST_IN_LIST_MSG_CN, MODEL_EXIST_IN_LIST_MSG_SPAN));
        Msgbox.exec();
        goto FINISH;
    }

    // create a new entry and add it to <cur_list_entries>
    NUM_LEFT_PIECES = model_2be_added->NUM_LEFT_PIECES(); // CANTIDAD

    NUM_PIECES_PER_BOX = model_2be_added->m_NUM_PIECES_PER_BOX;
    // compute number of avaliable boxes

    DESCRIPTION_SPAN = model_2be_added->m_DESCRIPTION_SPAN;
    DESCRIPTION_CN = model_2be_added->m_DESCRIPTION_CN;
    PRIZE = model_2be_added->m_PRICE;

    new_entry = EntryPtr (new Entry( NUM_LEFT_PIECES, NUM_PIECES_PER_BOX,
                                     MODELCODE_2be_Added, ContainerID_2be_Added,
                                     DESCRIPTION_SPAN, DESCRIPTION_CN,
                                     PRIZE));

    cur_list_entries.add_entry(new_entry);

    this->update_added_models_table();

FINISH:
    this->setEnabled(true);
}


/* 用户点击了一个cell，我们要选中整行, 查找被选中的货 */
void CreateListWin::on_added_models_table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    const auto& table = this->added_models_table;
    table->selectRow(row);
}


// delete the selected row in the <added_models_table>
void CreateListWin::on_remove_selected_model_btn_clicked()
{
    this->setDisabled(true);

    int row_idx;

    auto table = this->added_models_table; // this table is what we are interested in in this function

    QList<QTableWidgetItem *> selected_items = table->selectedItems();

    if(selected_items.size() == 0) goto Finish; // nothing is selected

    row_idx = selected_items[0]->row();
    table->removeRow(row_idx);
    cur_list_entries.remove_entry(row_idx);

Finish:
    this->setEnabled(true);
}


// remove all table contents in <added_models_table>
void CreateListWin::on_reset_added_models_table_btn_clicked()
{
    this->added_models_table->clearContents();
    this->added_models_table->setRowCount(0);

    cur_list_entries.clear_memory();
}


// the user want to adjust an item in the <added_models_table>, we init a new window for this operation
void CreateListWin::on_added_models_table_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    this->setDisabled(true); // disable this win when adjust win is shown

    adjust_list_item_win = QSharedPointer<Adjust_List_Item_Win> (new Adjust_List_Item_Win);

    EntryPtr entry = cur_list_entries.entries[row];
    ModelPtr model = entry->get_corresponding_model();
    if(model.isNull()){
        write_error_file("CreateListWin::on_added_models_table_cellDoubleClicked: model is empty!");
        this->setEnabled(true);
        return;
    }

    adjust_list_item_win->parent_win = this;
    adjust_list_item_win->set_model_and_entry(model, entry, row);
    adjust_list_item_win->set_GUI_values();

    adjust_list_item_win->show();

    this->setEnabled(true);
}


// clear client information
void CreateListWin::on_reset_client_info_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr,
                                  lan("确认操作", "Confirme la acción"),
                                  lan("你确定要清除客户信息吗？", "¿Estás seguro de que quieres purgar la información de los clientes?"),
                                  QMessageBox::Yes|QMessageBox::No);

    // Process the user's response
    if (reply == QMessageBox::Yes) {
        this->clear_client_info();
    }
}


// 当这个被点击后，尝试删除目前选中的 model
void CreateListWin::on_pushButton_edit_selected_model_clicked()
{
    this->setDisabled(true); // disable this win when adjust win is shown

    // get the row number of the current selected model
    auto selectedItems = added_models_table->selectedItems();
    if(selectedItems.size() == 0){
        // warn user that none is selected
        QMessageBox Msgbox(this);
        Msgbox.setText(lan("没有选中任何货物！", "¡No se seleccionaron productos!"));
        Msgbox.exec();

        this->setEnabled(true);
        return;
    }

    int row = selectedItems[0]->row();

    adjust_list_item_win = QSharedPointer<Adjust_List_Item_Win> (new Adjust_List_Item_Win);

    EntryPtr entry = cur_list_entries.entries[row];
    ModelPtr model = entry->get_corresponding_model();
    if(model.isNull()){
        write_error_file("CreateListWin::on_pushButton_edit_selected_model_clicked: no model is selected!");
        this->setEnabled(true);
        return;
    }

    adjust_list_item_win->parent_win = this;
    adjust_list_item_win->set_model_and_entry(model, entry, row);
    adjust_list_item_win->set_GUI_values();

    adjust_list_item_win->show();

    this->setEnabled(true);
}

void CreateListWin::on_searched_models_table_itemSelectionChanged()
{
    this->setDisabled(true);

    const auto& table = this->searched_models_table;

    QString MODELCODE;
    QString ContainerID;

    QList items = table->selectedItems();

    if(items.size() <= 0){
        return; // do nothing
    }

    MODELCODE = items[0]->text(); // index 0 is the MODEL_CODE
    ContainerID = items[1]->text().trimmed();

    this->selected_model_in_search_table = inventory.get_Model(MODELCODE, ContainerID);

    //Finish:
    this->setEnabled(true);
}


void CreateListWin::clear_tables()
{
    this->ui->model_code_for_search_LE->setText("");

    this->clear_added_models_table();
    this->clear_searched_models_table();

}

void CreateListWin::set_clientInfo(QSharedPointer<Client> client)
{
    this->ui->lineEdit_clientID->setText(client->m_ID);
    this->ui->lineEdit_CLIENTE->setText(client->m_clientName);
    this->ui->lineEdit_AGENTE->setText(client->m_AGENTE);
    this->ui->lineEdit_CIUDAD->setText(client->m_CIUDAD);
    this->ui->lineEdit_CONDICIONES->setText(client->m_CONDICIONES);
    this->ui->lineEdit_DOMICILIO->setText(client->m_DOMICILIO);
    this->ui->lineEdit_RFC->setText(client->m_RFC);
}


void CreateListWin::on_pushButton_autoFill_clicked()
{
    const QString clientID = this->ui->lineEdit_clientID->text().trimmed();
    const QString clientName = this->ui->lineEdit_CLIENTE->text().trimmed();

    if(clientID.isEmpty() && clientName.isEmpty()){
        QMessageBox msg;
        msg.setText(lan("请输入客户号码或者客户名！", "Por favor, introduzca su número de cliente o nombre de cliente."));
        msg.exec();
        return;
    }

    // 优先使用id
    if(!clientID.isEmpty()){
        QSharedPointer<Client> client = clientManager.get_client_by_ID(clientID);
        if(!client.isNull()){
            this->set_clientInfo(client);
        }
    }
    else{ // 使用名字搜索客户
        QVector<QSharedPointer<Client> > candidates;
        clientManager.get_clients_by_NamePrefix(clientName, candidates);
        if(candidates.size() != 0){
            this->set_clientInfo(candidates[0]);
        }
    }
}


void CreateListWin::on_pushButton_searchModel_clicked()
{
    this->setDisabled(true);

    this->selected_model_in_search_table = nullptr;

    QString userInput = this->ui->model_code_for_search_LE->text().trimmed(); // remove useless empty spaces

    auto table = this->searched_models_table;
    this->clear_searched_models_table();

    if(userInput.isEmpty()){
        // if input is empty, empty the table and return
        this->setEnabled(true);
        this->ui->model_code_for_search_LE->setFocus();
        return;
    }


    QVector<ModelPtr> models; // will hold the models that has MODELCODE starts with new_str
    inventory.searchModel_starts_with(userInput, models);

    // before search begins, display a dialog
    if(models.size() > 1000) QMessageBox::information(this, lan("搜索进行中", "La búsqueda está en curso"),
                             lan("搜索可能需要花一些时间，请稍等。", "La búsqueda puede llevar algún tiempo, así que espere."));

    // for each model, make a row for it
    for(unsigned long row = 0; row < models.size(); row++){
        const ModelPtr model = models[row];

        table->insertRow(table->rowCount());

        QVector<QString> items;
        model->searchResult_Regular(items);

        for( unsigned long col = 0; col < items.size(); col++ ){
            if (col == 2 || col == 3) { // Replace with QTextEdit when col reaches 2 or 3
                QTextEdit *textEdit = new QTextEdit();
                textEdit->setPlainText(items[col]);
                textEdit->setReadOnly(true); // Optionally make it read-only

                table->setCellWidget(row, col, textEdit);
            } else {
                QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
                tableWidgetItem->setText( items[col] );

                tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

                table->setItem(row, col, tableWidgetItem);
            }
        }
    }

    this->setEnabled(true);
    this->ui->model_code_for_search_LE->setFocus();
}

