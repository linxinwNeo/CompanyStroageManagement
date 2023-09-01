#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>

#include "CreateListWin.h"
#include "Others/handle_containerID.h"
#include "create_PDF.h"
#include "ui_CreateListWin.h"
#include "GlobalVars.h"

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


// create the pdf and deduc all listed items from the stroage
// also clear the <added_models_table>
void CreateListWin::on_generatePDF_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, PDF_MESSAGE_1, PDF_MESSAGE_2,
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return; // return if the user says no

    this->list = ListPtr(new List());

    // save things in the <added_models_table> to the <this->list> first
    for(int i = 0; i < this->added_models_table->rowCount(); i++){
        const double NUM_BOXES = this->added_models_table->item(i, added_models_table_NUM_BOXES_idx)->text().toDouble();
        const unsigned long TOTAL_NUM_ITEMS = this->added_models_table->item(i, added_models_table_NUM_ITEMS_idx)->text().toLong();
        const unsigned long NUM_ITEMS_PER_BOX = this->added_models_table->item(i, added_models_table_NUM_ITEMS_PER_BOX_idx)->text().toLong();
        const QString MODEL_CODE = this->added_models_table->item(i, added_models_table_MODELCODE_idx)->text();
        const QString DESCRIPTION_SPAN = this->added_models_table->item(i, added_models_table_DESCRIPTION_SPAN_idx)->text();
        const QString DESCRIPTION_CN = this->added_models_table->item(i, added_models_table_DESCRIPTION_CN_idx)->text();
        const double PRIZE = this->added_models_table->item(i, added_models_table_PRIZE_idx)->text().toDouble();
        const double TOTAL_PRIZE = this->added_models_table->item(i, added_models_table_TOTAL_idx)->text().toDouble();
        const QString ContainerID = this->added_models_table->item(i, added_models_table_ContainerID_idx)->text();
        EntryPtr new_entry(new Entry(NUM_BOXES, TOTAL_NUM_ITEMS, NUM_ITEMS_PER_BOX,
                                     MODEL_CODE, ContainerID,
                                     DESCRIPTION_SPAN, DESCRIPTION_CN,
                                     PRIZE, TOTAL_PRIZE));
        this->list->add_item(new_entry);
    }

    // save client info
    this->list->client_info.CLIENTE = this->ui->CLIENTE_LE->text();
    this->list->client_info.DOMICILIO = this->ui->DOMICILIO_LE->text();
    this->list->client_info.CIUDAD = this->ui->CIUDAD_LE->text();
    this->list->client_info.RFC = this->ui->RFC_LE->text();
    this->list->client_info.AGENTE = this->ui->AGENTE_LE->text();
    this->list->client_info.CONDICIONES = this->ui->CONDICIONES_LE->text();
    this->list->client_info.DISCOUNT = this->ui->discount_SB->value() / 100.; // the value the user is entering is between 0-100
    this->list->client_info.TOTAL_NUM_BOXES = this->list->total_num_boxes();
    const long int id = lists.get_unique_id();
    this->list->id = id;

    // ask for the path to store the file
    QString filter = tr("PDF (*.pdf)");
    QString filename = QFileDialog::getSaveFileName(this, GET_DESTINATION_MESSAGE, "list", filter);
    if(filename.isEmpty())
        return;

    QMessageBox Msgbox(this);

    // create PDF file
    create_pdf(filename, this->list);

    // display creation success file
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox.setText(".pdf 文件创建成功");
    Msgbox.exec();

    // now deduct the items from stroage and save the list
    //TODO

    // save the list to a txt file
    lists.add_list(this->list);
    lists.save_2_file();
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


// 用户输入了要搜索的货号前缀，搜索所有符合的货然后加入到表格里
void CreateListWin::on_model_code_for_search_LE_textChanged(const QString &new_str)
{
    this->setEnabled(false);

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
    searched_models_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    searched_models_table->setStyleSheet(table_stylesheet);

    added_models_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
    QVector<QString> items;

    this->setEnabled(false);
    if(this->selected_model_in_search_table.isNull()) goto FINISH;

    model_2be_added = this->selected_model_in_search_table;
    MODELCODE_2be_Added = model_2be_added->MODEL_CODE;

    if(this->selected_model_in_search_table->container.isNull()) ContainerID_2be_Added.clear();
    else ContainerID_2be_Added = model_2be_added->container->ID;

    // check if this model is in <added_models_table> already (we need to check both modelCODE and container)
    for(int row = 0; row < this->added_models_table->rowCount(); row ++){
        QString cur_modelCODE = this->added_models_table->item(row, added_models_table_MODELCODE_idx)->text(); // get the modelCODE for this row
        QString cur_containerID = this->added_models_table->item(row, added_models_table_ContainerID_idx)->text(); // get the containerID
        handle_ContainerID(cur_containerID);

        // if both matches, then the model already exists, we dont put it in
        if(cur_modelCODE == MODELCODE_2be_Added && cur_containerID == ContainerID_2be_Added) goto FINISH;
    }
    // we need to put the selected model into the <added_models_table>
    added_models_table->insertRow(added_models_table->rowCount());

    model_2be_added->searchResult_List(items);

    for( UI col = 0; col < items.size(); col++ ){
        QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
        tableWidgetItem->setText( items[col] );

        tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

        added_models_table->setItem(added_models_table->rowCount()-1, col, tableWidgetItem);
    }


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

    if(selected_items.size() != this->NUM_ADDED_MODELS_TABLE_COLS) return; // make sure the whole row is selected

    QString MODELCODE = selected_items[3]->text();
    QString CONTAINER_ID = selected_items[this->NUM_ADDED_MODELS_TABLE_COLS - 1]->text();
    ModelPtr model = inventory.get_Model(MODELCODE, CONTAINER_ID);
    if(model.isNull()){
        qDebug() << "CreateListWin::on_added_models_table_cellDoubleClicked: model is empty!";
        return;
    }
    
    adjust_list_item_win->parent_win = this;
    adjust_list_item_win->added_models_table = this->added_models_table;
    adjust_list_item_win->set_init_UI_values(model);

    this->setEnabled(false); // disable this win when adjust win is shown
    adjust_list_item_win->show();
}


// create a pdf but do not deduct items from the stroage
void CreateListWin::on_previewList_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, PDF_MESSAGE_1, PDF_MESSAGE_2,
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return; // return if the user says no

    this->list = ListPtr(new List());

    // save things in the <added_models_table> to the <this->list> first
    for(int i = 0; i < this->added_models_table->rowCount(); i++){
        const double NUM_BOXES = this->added_models_table->item(i, added_models_table_NUM_BOXES_idx)->text().toDouble();
        const unsigned long TOTAL_NUM_ITEMS = this->added_models_table->item(i, added_models_table_NUM_ITEMS_idx)->text().toLong();
        const unsigned long NUM_ITEMS_PER_BOX = this->added_models_table->item(i, added_models_table_NUM_ITEMS_PER_BOX_idx)->text().toLong();
        const QString MODEL_CODE = this->added_models_table->item(i, added_models_table_MODELCODE_idx)->text();
        const QString DESCRIPTION_SPAN = this->added_models_table->item(i, added_models_table_DESCRIPTION_SPAN_idx)->text();
        const QString DESCRIPTION_CN = this->added_models_table->item(i, added_models_table_DESCRIPTION_CN_idx)->text();
        const double PRIZE = this->added_models_table->item(i, added_models_table_PRIZE_idx)->text().toDouble();
        const double TOTAL_PRIZE = this->added_models_table->item(i, added_models_table_TOTAL_idx)->text().toDouble();
        const QString ContainerID = this->added_models_table->item(i, added_models_table_ContainerID_idx)->text();
        EntryPtr new_entry(new Entry(NUM_BOXES, TOTAL_NUM_ITEMS, NUM_ITEMS_PER_BOX,
                                     MODEL_CODE, ContainerID,
                                     DESCRIPTION_SPAN, DESCRIPTION_CN,
                                     PRIZE, TOTAL_PRIZE));
        this->list->add_item(new_entry);
    }

    // save client info
    this->list->client_info.CLIENTE = this->ui->CLIENTE_LE->text();
    this->list->client_info.DOMICILIO = this->ui->DOMICILIO_LE->text();
    this->list->client_info.CIUDAD = this->ui->CIUDAD_LE->text();
    this->list->client_info.RFC = this->ui->RFC_LE->text();
    this->list->client_info.AGENTE = this->ui->AGENTE_LE->text();
    this->list->client_info.CONDICIONES = this->ui->CONDICIONES_LE->text();
    this->list->client_info.DISCOUNT = this->ui->discount_SB->value() / 100.; // the value the user is entering is between 0-100
    this->list->client_info.TOTAL_NUM_BOXES = this->list->total_num_boxes();

    // ask for the path to store the file
    QString filter = tr("PDF (*.pdf)");
    QString filename = QFileDialog::getSaveFileName(this, GET_DESTINATION_MESSAGE, "list", filter);
    if(filename.isEmpty())
        return;

    QMessageBox Msgbox(this);

    // create PDF file
    create_pdf(filename, this->list);

    // display creation success file
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox.setText("清单创建成功");
    Msgbox.exec();
}

