#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

#include "CreateListWin.h"
#include "Others/get_save_filePath.h"
#include "Others/handle_containerID.h"
#include "Others/create_PDF.h"
#include "ui_CreateListWin.h"
#include "GlobalVars.h"

EntryList cur_list_entries;

CreateListWin::CreateListWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateListWin)
{
    ui->setupUi(this);

    this->init();
}


CreateListWin::~CreateListWin()
{
    delete ui;
}


void CreateListWin::update_added_models_table()
{
    this->setDisabled(true);

    this->added_models_table->clearContents();
    this->added_models_table->setRowCount(0);

    // for each entry, make a row for it
    for(UI row = 0; row < cur_list_entries.num_entries(); row++){
        const EntryPtr entry = cur_list_entries.get_entry(row);
        if(entry.isNull()) continue;

        added_models_table->insertRow(added_models_table->rowCount());

        auto rowValues = entry->view_values();

        for( UI col = 0; col < rowValues.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( rowValues[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            added_models_table->setItem(row, col, tableWidgetItem);
        }
    }

Finish:
    this->setEnabled(true);
}


// create the pdf and deduct all listed items from the stroage
// also clear the <added_models_table>
void CreateListWin::on_generatePDF_btn_clicked()
{
    if(cur_list_entries.num_entries() == 0) return;

    this->setDisabled(true);

    QString filter;
    QString filePath;
    QMessageBox Msgbox(this);
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox.setText("清单创建成功");
    const unsigned long int unused_unique_id = lists.get_unique_id();

    QMessageBox::StandardButton reply = QMessageBox::question(this, PDF_MESSAGE_1, PDF_MESSAGE_2,
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) goto Finish; // return if the user says no

    this->list = ListPtr(new List());

    this->list->itemList = cur_list_entries;

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

    list->date_created = QDate::currentDate();
    list->time_created = QTime::currentTime();

    // ask for the path to store the file
    filter = tr("PDF (*.pdf)");
    filePath = get_save_filePath("list.pdf", WHERE_TO_SAVE_FILE_MESSAGE, filter);

    if(filePath.isEmpty()) goto Finish;

    // create PDF file
    create_pdf(filePath, this->list);

    // display creation success
    Msgbox.exec();

    // now deduct the items from stroage
    inventory.deduct_models(this->list->itemList.entries);

    // save the list to a txt file
    lists.add_list(this->list);
    lists.save_2_file();

    cur_list_entries.clear_memory();
    this->added_models_table->clearContents();
    this->added_models_table->setRowCount(0);
    this->on_model_code_for_search_LE_textChanged(this->ui->model_code_for_search_LE->text()); // 更新<search_models_table>
    this->selected_model_in_search_table = nullptr; // reset selected model

Finish:
    this->setEnabled(true);
}


// create a pdf but do not deduct items from the stroage
void CreateListWin::on_previewList_btn_clicked()
{
    QString filter;
    QString filePath;
    QMessageBox Msgbox(this);
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");

    if(cur_list_entries.num_entries() == 0) {
        Msgbox.setText("清单是空的");
        Msgbox.exec();
    }

    this->setDisabled(true);

    QMessageBox::StandardButton reply = QMessageBox::question(this, PDF_MESSAGE_1, PDF_MESSAGE_2,
                                                            QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) goto Finish;

    this->list = ListPtr(new List());

    this->list->itemList = cur_list_entries;

    // save client info
    this->list->client_info.CLIENTE = this->ui->CLIENTE_LE->text();
    this->list->client_info.DOMICILIO = this->ui->DOMICILIO_LE->text();
    this->list->client_info.CIUDAD = this->ui->CIUDAD_LE->text();
    this->list->client_info.RFC = this->ui->RFC_LE->text();
    this->list->client_info.AGENTE = this->ui->AGENTE_LE->text();
    this->list->client_info.CONDICIONES = this->ui->CONDICIONES_LE->text();
    this->list->client_info.DISCOUNT = this->ui->discount_SB->value() / 100.; // the value the user is entering is between 0-100
    this->list->client_info.TOTAL_NUM_BOXES = this->list->total_num_boxes();
    this->list->date_created = QDate::currentDate();
    this->list->time_created = QTime::currentTime();

    // ask for the path to store the file
    filter = tr("PDF (*.pdf)");
    filePath = get_save_filePath("list.pdf", WHERE_TO_SAVE_FILE_MESSAGE, filter);

    if(filePath.isEmpty()) goto Finish;

    // create PDF file
    create_pdf(filePath, this->list);

    // display creation success
    Msgbox.setText("清单创建成功");
    Msgbox.exec();

Finish:
    this->setEnabled(true);
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
    for(UI row = 0; row < models.size(); row++){
        const ModelPtr model = models[row];

        table->insertRow(table->rowCount());

        QVector<QString> items;
        model->searchResult_Regular(items);

        for( UI col = 0; col < items.size(); col++ ){
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

    const auto& table = this->searched_models_table;
    table->selectRow(row);

    QList items = table->selectedItems();
    if(items.length() != this->NUM_SEARCHED_MODELS_TABLE_COLS) return;

    QString MODELCODE = items[searched_models_table_MODELCODE_idx]->text(); // index 0 is the MODEL_CODE
    QString ContainerID = items[searched_models_table_ContainerID_idx]->text();
    // set ID to empty if this model does not have a container
    handle_ContainerID(ContainerID);

    this->selected_model_in_search_table = inventory.get_Model(MODELCODE, ContainerID);
}


/* try to add selected model in to the <added_models_table>, but we need to check if it has been added already */
void CreateListWin::on_add_selected_model_btn_clicked()
{
    ModelPtr model_2be_added;
    QString MODELCODE_2be_Added;
    QString ContainerID_2be_Added;

    double NUM_BOXES;
    unsigned long TOTAL_NUM_ITEMS;
    unsigned long NUM_ITEMS_PER_BOX;
    QString MODEL_CODE;
    QString DESCRIPTION_SPAN;
    QString DESCRIPTION_CN;
    double PRIZE;
    double TOTAL_PRIZE;
    QString ContainerID;
    EntryPtr new_entry;

    this->setDisabled(true);

    if(this->selected_model_in_search_table.isNull()) goto FINISH;

    model_2be_added = this->selected_model_in_search_table;
    MODELCODE_2be_Added = model_2be_added->MODEL_CODE;

    if(this->selected_model_in_search_table->container.isNull()) ContainerID_2be_Added.clear();
    else ContainerID_2be_Added = model_2be_added->container->ID;

    // check if this model has been added to <added_models_table> already
    if(cur_list_entries.has_Model(MODELCODE_2be_Added, ContainerID_2be_Added)){
        goto FINISH;
    }

    // create a new entry and add it to <cur_list_entries>
    NUM_BOXES = model_2be_added->NUM_LEFT_BOXES;
    TOTAL_NUM_ITEMS = model_2be_added->NUM_LEFT_ITEMS;
    NUM_ITEMS_PER_BOX = model_2be_added->NUM_ITEMS_PER_BOX;
    MODEL_CODE = model_2be_added->MODEL_CODE;
    DESCRIPTION_SPAN = model_2be_added->DESCRIPTION_SPAN;
    DESCRIPTION_CN = model_2be_added->DESCRIPTION_CN;
    PRIZE = model_2be_added->PRIZE;
    TOTAL_PRIZE = model_2be_added->TOTAL_PRIZE(model_2be_added->NUM_LEFT_BOXES);
    ContainerID = ContainerID_2be_Added;
    new_entry = EntryPtr (new Entry(NUM_BOXES, TOTAL_NUM_ITEMS, NUM_ITEMS_PER_BOX,
                                 MODEL_CODE, ContainerID,
                                 DESCRIPTION_SPAN, DESCRIPTION_CN,
                                 PRIZE, TOTAL_PRIZE));

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
    this->setEnabled(false);

    auto table = this->added_models_table; // this table is what we are interested in in this function

    QList<QTableWidgetItem *> selected_items = table->selectedItems();

    if(selected_items.size() == 0) return; // nothing is selected

    int row_idx = selected_items[0]->row();
    table->removeRow(row_idx);

    this->setEnabled(true);
}


// remove all table contents in <added_models_table>
void CreateListWin::on_reset_added_models_table_btn_clicked()
{
    this->added_models_table->clearContents();
    this->added_models_table->setRowCount(0);
}


// the user want to adjust an item in the <added_models_table>, we init a new window for this operation
void CreateListWin::on_added_models_table_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    adjust_list_item_win = QSharedPointer<Adjust_List_Item_Win> (new Adjust_List_Item_Win);

    // get the selected row
    added_models_table->selectRow(row);
    QList<QTableWidgetItem*> selected_items = this->added_models_table->selectedItems();

    QString MODELCODE = selected_items[added_models_table_MODELCODE_idx]->text();
    QString CONTAINER_ID = selected_items[added_models_table_ContainerID_idx]->text();
    ModelPtr model = inventory.get_Model(MODELCODE, CONTAINER_ID);
    if(model.isNull()){
        qDebug() << "CreateListWin::on_added_models_table_cellDoubleClicked: model is empty!";
        return;
    }

    const unsigned long NUM_ITEMS = selected_items[added_models_table_NUM_ITEMS_idx]->text().toULong();
    
    adjust_list_item_win->parent_win = this;
    adjust_list_item_win->added_models_table = this->added_models_table;
    adjust_list_item_win->set_init_UI_values(model, NUM_ITEMS);

    this->setEnabled(false); // disable this win when adjust win is shown
    adjust_list_item_win->show();
}


