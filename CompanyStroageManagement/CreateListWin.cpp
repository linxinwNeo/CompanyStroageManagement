#include "CreateListWin.h"
#include "ui_CreateListWin.h"

#include "helper_functions.h"
#include "GlobalVars.h"
#include "flags.h"
#include "QXlsx/Excel.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "FileLoader/ReadFile.h"

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


void CreateListWin::on_generatePDF_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, PDF_MESSAGE_1, PDF_MESSAGE_2,
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return; // return if the user says no

    this->list = ListPtr(new List());

    // save things in the <added_models_table> to the <this->list> first
    for(int i = 0; i < this->added_models_table->rowCount(); i++){
        const double NUM_BOXES = this->added_models_table->item(i, 0)->text().toDouble();
        const unsigned long TOTAL_NUM_ITEMS = this->added_models_table->item(i, 1)->text().toLong();
        const unsigned long NUM_ITEMS_PER_BOX = this->added_models_table->item(i, 2)->text().toLong();
        const QString MODEL_CODE = this->added_models_table->item(i, 3)->text();
        const QString DESCRIPTION_SPAN = this->added_models_table->item(i, 4)->text();
        const double PRIZE = this->added_models_table->item(i, 5)->text().toDouble();
        const double TOTAL_PRIZE = this->added_models_table->item(i, 6)->text().toDouble();
        EntryPtr new_entry(new Entry(NUM_BOXES, TOTAL_NUM_ITEMS, NUM_ITEMS_PER_BOX, MODEL_CODE,
                                     DESCRIPTION_SPAN, PRIZE, TOTAL_PRIZE));
        this->list->add_item(new_entry);
    }

    // save client info
    this->list->client_info.CLIENTE = this->ui->CLIENTE_LE->text();
    this->list->client_info.DOMICILIO = this->ui->DOMICILIO_LE->text();
    this->list->client_info.CIUDAD = this->ui->CIUDAD_LE->text();
    this->list->client_info.RFC = this->ui->RFC_LE->text();
    this->list->client_info.AGENTE = this->ui->AGENTE_LE->text();
    this->list->client_info.CONDICIONES = this->ui->CONDICIONES_LE->text();
    this->list->client_info.DISCOUNT = this->ui->discount_SB->value();
    this->list->client_info.TOTAL_NUM_BOXES = this->list->total_num_boxes();

    // ask for the path to store the file
    QString filter = tr("PDF (*.pdf)");
    QString filename = QFileDialog::getSaveFileName(this, GET_DESTINATION_MESSAGE, "list", filter);
    if(filename.isEmpty())
        return;

    QMessageBox Msgbox(this);

    // create PDF file
    this->create_pdf(filename);

    // display creation success file
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox.setText(".pdf 文件创建成功");
    Msgbox.exec();
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
    this->selected_model_in_added_table = nullptr;
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

    QString MODELCODE = items[0]->text(); // index 0 is the MODEL_CODE
    QString ContainerID = items[items.length()-1]->text();
    // set ID to empty if this model does not have a container
    if(ContainerID == none_CN || ContainerID == none_SPAN) ContainerID.clear();

    this->selected_model_in_search_table = inventory.get_Model(MODELCODE, ContainerID);
}


/* try to add selected model in to the added_table, but we need to check if it has been added already */
void CreateListWin::on_add_selected_model_btn_clicked()
{
    if(this->selected_model_in_search_table.isNull()) return;

    QString MODELCODE_2be_Added = this->selected_model_in_search_table->MODEL_CODE;
    QString ContainerID_2be_Added;
    if(this->selected_model_in_search_table->container.isNull()) ContainerID_2be_Added.clear();
    else ContainerID_2be_Added = this->selected_model_in_search_table->container->ID;

    // check if this model is in <added_models_table> already (we need to check both modelCODE and container)
    for(int row = 0; row < this->added_models_table->rowCount(); row ++){
        QString cur_modelCODE = this->added_models_table->item(row, 3)->text(); // get the modelCODE for this row
        QString cur_containerID = this->added_models_table->item(row, 7)->text(); // get the containerID
        if(cur_containerID == none_CN || cur_containerID == none_SPAN) cur_containerID.clear();
        // if both matches, then the model already exists, we dont put it in
        if(cur_modelCODE == MODELCODE_2be_Added && cur_containerID == ContainerID_2be_Added){
            return;
        }
    }

    // we need to put this model into the <added_models_table>
    const ModelPtr model_2be_added = this->selected_model_in_search_table;

    added_models_table->insertRow(added_models_table->rowCount());

    QVector<QString> items;
    model_2be_added->searchResult_List(items);

    for( UI col = 0; col < items.size(); col++ ){
        QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
        tableWidgetItem->setText( items[col] );

        tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

        added_models_table->setItem(added_models_table->rowCount()-1, col, tableWidgetItem);
    }
}


/* 用户点击了一个cell，我们要选中整行, 查找被选中的货 */
void CreateListWin::on_added_models_table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    const auto& table = this->added_models_table;
    table->selectRow(row);

    QList items = table->selectedItems();
    if(items.length() != this->NUM_ADDED_MODELS_TABLE_COLS) return;

    QString MODELCODE = items[3]->text(); // index 3 is the MODEL_CODE in added model table
    QString ContainerID = items[items.length()-1]->text();

    this->selected_model_in_added_table = inventory.get_Model(MODELCODE, ContainerID);
}


// delete the selected model in added table
// we remove at most one row at a time, but a table should have only one entry for the model
void CreateListWin::on_remove_selected_model_btn_clicked()
{
    auto selected_model = this->selected_model_in_added_table; // this model is what we are interested in in this function

    if(selected_model.isNull()) return;

    auto table = this->added_models_table; // this table is what we are interested in in this function

    QString MODELCODE = selected_model->MODEL_CODE;
    QString ContainerID;
    if(selected_model->container.isNull()) ContainerID.clear();
    else ContainerID = selected_model->container->ID;

    // look for the correspodning row of this model
    for(int row = 0; row < table->rowCount(); row ++){
        QString cur_modelCODE = table->item(row, 3)->text(); // get the modelCODE for this row
        QString cur_containerID = table->item(row, 7)->text(); // get the containerID

        if(cur_containerID == none_CN || cur_containerID == none_SPAN) cur_containerID.clear();
        // if both matches, then we remove this model
        if(cur_modelCODE == MODELCODE && cur_containerID == ContainerID){
            table->removeRow(row);
            this->selected_model_in_added_table = nullptr;
            return;
        }
    }

    // if we don't find any model that matches, we shall return
    return;
}


// remove all table contents
void CreateListWin::on_reset_added_models_table_btn_clicked()
{
    this->selected_model_in_added_table = nullptr;
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

    adjust_list_item_win->set_parentWin(this);
    adjust_list_item_win->set_init_UI_values(model);
    adjust_list_item_win->selected_items = selected_items;

    this->setEnabled(false); // disable this win when adjust win is shown
    adjust_list_item_win->show();
}

