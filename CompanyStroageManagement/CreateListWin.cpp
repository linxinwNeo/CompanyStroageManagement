#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

#include "CN_Strings.h"
#include "CreateListWin.h"
#include "FileLoader/WriteFile.h"
#include "Others/create_PDF.h"
#include "SpanStrings.h"
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

    this->added_models_table->clearContents();
    this->added_models_table->setRowCount(0);

    // for each entry, make a row for it
    for(unsigned long row = 0; row < cur_list_entries.num_entries(); row++){
        const EntryPtr entry = cur_list_entries.get_entry(row);
        if(entry.isNull()) continue;

        added_models_table->insertRow(added_models_table->rowCount());

        auto rowValues = entry->view_values();

        for( unsigned long col = 0; col < rowValues.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( rowValues[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            added_models_table->setItem(row, col, tableWidgetItem);
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

    this->ui->CLIENTE_label->setText(lan("客户", "CLIENTE"));
    this->ui->CLIENTE_LE->setPlaceholderText(enter_here);

    this->ui->DOMICILIO_label->setText(lan("地址", "DOMICILIO"));
    this->ui->DOMICILIO_LE->setPlaceholderText(enter_here);

    this->ui->CIUDAD_label->setText(lan("城市", "CIUDAD"));
    this->ui->CIUDAD_LE->setPlaceholderText(enter_here);

    this->ui->RFC_label->setText("R.F.C.");
    this->ui->RFC_LE->setPlaceholderText(enter_here);

    this->ui->AGENTE_label->setText(lan("代理", "AGENTE"));
    this->ui->AGENTE_LE->setPlaceholderText(enter_here);

    this->ui->CONDICIONES_label->setText(lan("付款方式", "CONDICIONES DE PAGO"));
    this->ui->CONDICIONES_LE->setPlaceholderText(enter_here);

    this->ui->discount_label->setText(lan("折扣", "DISCOUNT"));

    this->ui->reset_client_info_btn->setText(lan("重置信息", "resetear la información"));

    this->ui->searchModel_TW->setTabText(0, lan("使用货号搜索需要添加的货物", "buscar los productos a agregar utilizando el número de referencia"));

    this->ui->model_code_for_search_label->setText(lan("货号", "MODELO"));
    this->ui->model_code_for_search_LE->setPlaceholderText(lan("在此输入需要搜索的货号 比如 TA-0001", "Introduce el número de referencia que deseas buscar aquí, por ejemplo, TA-0001"));

    QStringList headers = {
        lan("货号", "MODELO"),
        lan("集装箱号", "Número de contenedor"),
        lan("品名(中文)", "Nombre del producto (en chino)"),
        lan("品名(西语)", "Nombre del producto (en español)"),

        lan("进货箱数", "Núm. cajas adq."),
        lan("已售箱数", "Núm. cajas vend."),
        lan("剩余箱数", "Núm. cajas rest."),

        lan("每箱个数", "Piezas por caja"),

        lan("进货个数", "Núm. piezas adq."),
        lan("已售个数", "Núm. piezas vend."),
        lan("剩余个数", "Núm. unidades rest."),

        lan("单价", "Prec. por pieza"),
        lan("上次修改时间", "Fecha últ. modif."),
    };

    this->ui->searched_models_table->setHorizontalHeaderLabels(headers);

    this->ui->add_selected_model_btn->setText(lan("添加选中的货物", "añadir los productos seleccionados"));

    this->ui->added_models_GB->setTitle(lan("已添加的货物（双击货物修改数量）", "Productos añadidos (doble clic para modificar la cantidad)"));

    this->ui->remove_selected_model_btn->setText(lan("删除选中的货物", "eliminar los productos seleccionados"));

    this->ui->reset_added_models_table_btn->setText(lan("删除所有已添加的货物", "eliminar todos los productos añadidos"));

    this->ui->generate_list_GB->setTitle(lan("生成清单", "crear lista"));

    this->ui->previewList_btn->setText(lan("生成预览清单", "crear una vista previa de la lista"));

    this->ui->generatePDF_btn->setText(lan("生成正式清单", "Genere una lista de verificación formal"));
}


// create the pdf and deduct all listed items from the stroage
// also clear the <added_models_table>
void CreateListWin::on_generatePDF_btn_clicked()
{
    QString filePath;
    QMessageBox Msgbox;
    QFileDialog saveFileDialog;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    if(cur_list_entries.num_entries() == 0) {
        Msgbox.setText(lan("清单是空的", "La lista está vacía"));
        Msgbox.exec();
        return;
    }

    const unsigned long int unused_unique_id = lists.get_unique_id();

    QString PDF_MSG_1 = lan(PDF_MESSAGE_1_CN, PDF_MESSAGE_1_SPAN);
    QString PDF_MSG_2 = lan(PDF_MESSAGE_2_CN, PDF_MESSAGE_2_SPAN);

    QMessageBox::StandardButton reply = QMessageBox::question(this, PDF_MSG_1, PDF_MSG_2,
                                                              QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) goto Finish; // return if the user says no

    this->list = ListPtr(new List());
    
    this->list->entryList = cur_list_entries;

    // save client info
    this->list->client_info.CLIENTE = this->ui->CLIENTE_LE->text();
    this->list->client_info.DOMICILIO = this->ui->DOMICILIO_LE->text();
    this->list->client_info.CIUDAD = this->ui->CIUDAD_LE->text();
    this->list->client_info.RFC = this->ui->RFC_LE->text();
    this->list->client_info.AGENTE = this->ui->AGENTE_LE->text();
    this->list->client_info.CONDICIONES = this->ui->CONDICIONES_LE->text();
    this->list->client_info.DISCOUNT = this->ui->discount_SB->value() / 100.; // the value the user is entering is between 0-100
    this->list->client_info.TOTAL_NUM_BOXES = this->list->total_num_boxes();
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

    if(filePath.isEmpty()) goto Finish;

    // create PDF file
    create_pdf(filePath, this->list);

    // now deduct the items from stroage
    inventory.deduct_models(this->list->entryList.entries);

    // save the list to a txt file
    lists.add_list(this->list);
    lists.save_2_file(false);

    cur_list_entries.clear_memory();
    this->added_models_table->clearContents();
    this->added_models_table->setRowCount(0);
    this->on_model_code_for_search_LE_textChanged(this->ui->model_code_for_search_LE->text()); // 更新<search_models_table>
    this->selected_model_in_search_table = nullptr; // reset selected model

Finish:
    // save the inventory and lists
    WriteFile::SaveInventoryAuto(false);
    WriteFile::Lists2txt(false);

    // display creation success
    Msgbox.setText(lan(LIST_CREATED_MSG_CN, LIST_CREATED_MSG_SPAN));
    Msgbox.exec();

    // reset client information
    this->on_reset_client_info_btn_clicked();
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
    this->list->client_info.CLIENTE = this->ui->CLIENTE_LE->text();
    this->list->client_info.DOMICILIO = this->ui->DOMICILIO_LE->text();
    this->list->client_info.CIUDAD = this->ui->CIUDAD_LE->text();
    this->list->client_info.RFC = this->ui->RFC_LE->text();
    this->list->client_info.AGENTE = this->ui->AGENTE_LE->text();
    this->list->client_info.CONDICIONES = this->ui->CONDICIONES_LE->text();
    this->list->client_info.DISCOUNT = this->ui->discount_SB->value() / 100.; // the value the user is entering is between 0-100
    this->list->client_info.TOTAL_NUM_BOXES = this->list->total_num_boxes();
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


// 用户输入了要搜索的货号前缀，搜索所有符合的货然后加入到表格里
void CreateListWin::on_model_code_for_search_LE_textChanged(const QString &new_str)
{
    this->setDisabled(true);

    this->selected_model_in_search_table = nullptr;

    QString userInput = new_str.trimmed(); // remove useless empty spaces

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

    // for each model, make a row for it
    for(unsigned long row = 0; row < models.size(); row++){
        const ModelPtr model = models[row];

        table->insertRow(table->rowCount());

        QVector<QString> items;
        model->searchResult_Regular(items);

        for( unsigned long col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            table->setItem(row, col, tableWidgetItem);
        }
    }

    this->setEnabled(true);
    this->ui->model_code_for_search_LE->setFocus();
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

    //设置表格 style
//    searched_models_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    searched_models_table->setStyleSheet(table_stylesheet);

//    added_models_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    added_models_table->setStyleSheet(table_stylesheet);
}


// 清理表格数据
void CreateListWin::clear_searched_models_table()
{
    if(!this->searched_models_table) return;
    searched_models_table->clearContents(); // clear the table contents but columns are reserved
    searched_models_table->setRowCount(0);
}


// 清理表格数据
void CreateListWin::clear_added_models_table()
{
    if(!this->added_models_table) return;
    added_models_table->clearContents(); // clear the table contents but columns are reserved
    added_models_table->setRowCount(0);
}


/* 用户点击了一个cell，我们要选中整行, 查找被选中的货 */
void CreateListWin::on_searched_models_table_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    this->setDisabled(true);

    const auto& table = this->searched_models_table;
    table->selectRow(row);

    QString MODELCODE;
    QString ContainerID;

    QList items = table->selectedItems();

    MODELCODE = items[0]->text(); // index 0 is the MODEL_CODE
    ContainerID = items[1]->text().trimmed();

    this->selected_model_in_search_table = inventory.get_Model(MODELCODE, ContainerID);

//Finish:
    this->setEnabled(true);
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
    MODELCODE_2be_Added = model_2be_added->MODEL_CODE;

    if(this->selected_model_in_search_table->container.isNull()) ContainerID_2be_Added.clear();
    else ContainerID_2be_Added = model_2be_added->container->ID;

    // check if this model has been added to <selected_list_entries_Table> already
    if(cur_list_entries.has_Model(MODELCODE_2be_Added, ContainerID_2be_Added)){
        Msgbox.setText(lan(MODEL_EXIST_IN_LIST_MSG_CN, MODEL_EXIST_IN_LIST_MSG_SPAN));
        Msgbox.exec();
        goto FINISH;
    }

    // create a new entry and add it to <cur_list_entries>
    NUM_LEFT_PIECES = model_2be_added->NUM_LEFT_PIECES(); // CANTIDAD

    NUM_PIECES_PER_BOX = model_2be_added->NUM_PIECES_PER_BOX;
    // compute number of avaliable boxes

    DESCRIPTION_SPAN = model_2be_added->DESCRIPTION_SPAN;
    DESCRIPTION_CN = model_2be_added->DESCRIPTION_CN;
    PRIZE = model_2be_added->PRIZE;

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
}


// clear client information
void CreateListWin::on_reset_client_info_btn_clicked()
{
    this->ui->CLIENTE_LE->setText("");
    this->ui->DOMICILIO_LE->setText("");
    this->ui->CIUDAD_LE->setText("");
    this->ui->RFC_LE->setText("");
    this->ui->AGENTE_LE->setText("");
    this->ui->CONDICIONES_LE->setText("");
    this->ui->discount_SB->setValue(0.0);
}

