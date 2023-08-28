#include "mainwindow.h"
#include "FileLoader/ReadFile.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "GlobalVars.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->init();
}


MainWindow::~MainWindow()
{
    this->AddNewModelWinPtr = nullptr;
    this->CreateListWinPtr = nullptr;
    this->selected_model = nullptr;
    this->selected_container = nullptr;
    delete ui;
}


void MainWindow::init()
{
    this->AddNewModelWinPtr = nullptr;
    this->CreateListWinPtr = nullptr;
    this->selected_model = nullptr;
    this->selected_container = nullptr;

    // select the first tab initially
    this->ui->tabWidget->setCurrentIndex(0);

    // setting up tables
    auto container_table = ui->search_container_result_Table;
    container_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    auto model_table = ui->search_model_result_Table;
//    model_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model_table->setStyleSheet(table_stylesheet);

    auto selected_container_table = ui->selected_container_Table;
//    selected_container_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    selected_container_table->setStyleSheet(table_stylesheet);

    // read inventory.txt file
    ReadFile read_file;
    read_file.read_Inventory_txt_File(DB_FNAME); // build the inventory
}


void MainWindow::clear_search_container_result_table()
{
    auto table = this->ui->search_container_result_Table;
    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);

    this->clear_selected_container_table();
    this->selected_container = nullptr;
}


// show the selected model in the selected model group box
void MainWindow::show_selected_model()
{
    if(this->selected_model.isNull()) return;

    ui->selected_model_MODELCODE_LE->setText(this->selected_model->MODEL_CODE);
    ui->selected_model_DESCRIPTION_CN_LE->setText(this->selected_model->DESCRIPTION_CN);
    ui->selected_model_DESCRIPTION_SPAN_LE->setText(this->selected_model->DESCRIPTION_SPAN);
    ui->selected_model_NUM_INIT_BOXES_SB->setValue(this->selected_model->NUM_INIT_BOXES);
    ui->selected_model_NUM_SOLD_BOXES_SB->setValue(this->selected_model->NUM_SOLD_BOXES);
    ui->selected_model_NUM_ITEMS_PER_BOX_SB->setValue(this->selected_model->NUM_ITEMS_PER_BOX);
    ui->selected_model_PRIZE_SB->setValue(this->selected_model->NUM_SOLD_BOXES);

    if(this->selected_model->container.isNull()) ui->selected_model_CONTAINER_LE->setText(none_CN);
    else ui->selected_model_CONTAINER_LE->setText(this->selected_model->container->ID);
}


// reset this selected_model_GB, deselect model
void MainWindow::clear_selected_model()
{
    const QString empty;
    const double zero = 0.;
    ui->selected_model_MODELCODE_LE->setText(empty);
    ui->selected_model_DESCRIPTION_CN_LE->setText(empty);
    ui->selected_model_DESCRIPTION_SPAN_LE->setText(empty);
    ui->selected_model_NUM_INIT_BOXES_SB->setValue(zero);
    ui->selected_model_NUM_SOLD_BOXES_SB->setValue(zero);
    ui->selected_model_NUM_ITEMS_PER_BOX_SB->setValue(zero);
    ui->selected_model_PRIZE_SB->setValue(zero);
    ui->selected_model_CONTAINER_LE->setText(empty);

    this->selected_model = nullptr;
}

void MainWindow::clear_search_model_result_table()
{
    auto table = this->ui->search_model_result_Table;
    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);
}


/* for each model in the <selected_container>, make a row for it and add the row to the <selected_container_table> */
void MainWindow::show_selected_container()
{
    if(this->selected_container.isNull()) return;

    this->clear_selected_container_table();

    auto table = this->ui->selected_container_Table;
    QVector<ModelPtr> models;
    this->selected_container->models_Set2Vec(models, true); // sorted models

    // for each model, make a row for it
    UI row = 0;
    for(const ModelPtr& model : models){
        table->insertRow(table->rowCount());

        QVector<QString> items;
        model->searchResult_Regular(items);

        for( UI col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            table->setItem(row, col, tableWidgetItem);
        }

        row++;
    }
}


/* deselect the container, clear the selected_container_table */
void MainWindow::clear_selected_container_table()
{
    auto table = this->ui->selected_container_Table;
    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);
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


/* the user is typing in MODELCODE, we need to search the models starting with the same string and display them in
 *  the searchResult table */
void MainWindow::on_search_MODELCODE_LE_textChanged(const QString& new_str)
{
    this->setEnabled(false);

    QString userInput = new_str.trimmed(); // remove useless empty spaces

    auto table = this->ui->search_model_result_Table;
    this->clear_search_model_result_table();

    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);
    if(userInput.isEmpty()){
        // if input is empty, empty the table and return
        this->setEnabled(true);
        this->ui->search_MODELCODE_LE->setFocus();
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
    this->ui->search_MODELCODE_LE->setFocus();
}


/* update the model with the info entered */
void MainWindow::on_update_selected_model_btn_clicked()
{
    this->setEnabled(false);

    // make sure user is indeed wanting to update the values
    QMessageBox msg(this);
    msg.setText(tr("你确定更改吗?\n"));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::No);
    msg.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::No) {
        this->setEnabled(true);
        return;
    }

    selected_model->DESCRIPTION_CN = ui->selected_model_DESCRIPTION_CN_LE->text();
    selected_model->DESCRIPTION_SPAN = ui->selected_model_DESCRIPTION_SPAN_LE->text();
    selected_model->NUM_INIT_BOXES = ui->selected_model_NUM_INIT_BOXES_SB->value();
    selected_model->NUM_SOLD_BOXES = ui->selected_model_NUM_SOLD_BOXES_SB->value();
    selected_model->PRIZE = ui->selected_model_PRIZE_SB->value();
    selected_model->NUM_ITEMS_PER_BOX = ui->selected_model_NUM_ITEMS_PER_BOX_SB->value();

    selected_model->NUM_LEFT_BOXES = selected_model->NUM_INIT_BOXES - selected_model->NUM_SOLD_BOXES;
    selected_model->NUM_LEFT_ITEMS = selected_model->NUM_INIT_BOXES * selected_model->NUM_ITEMS_PER_BOX;

    const QString container_ID = ui->selected_model_CONTAINER_LE->text();

    if(container_ID == none_CN || container_ID == none_SPAN) // the container is none
    {
        // check if the model has container previsously
        if(selected_model->container.isNull()) goto LL;
        else{ // the selected_model has container previously, we need to remove this model from its container
            this->selected_model->container->remove_model(this->selected_model);
            this->selected_model->container = nullptr;
        }
    }
    else{ // the container exists
        // check if this container exists, if not, create one
        ContainerPtr orig_container = selected_model->container;
        ContainerPtr cur_container = inventory.get_container(container_ID);
        if(cur_container.isNull()){ // if no such container exists, create one
            cur_container = ContainerPtr (new Container(container_ID));
            cur_container->add_model(this->selected_model); // add the selected model to the container
            selected_model->container = cur_container;
        }

        // check if the container ID the same as before
        if(container_ID == orig_container->ID){
            goto LL;
        }
        else{
            // the container for the model has been modified

            // remove selected_model from the original container
            orig_container->remove_model(selected_model);
            selected_model->container = cur_container;
        }
    }

    LL:
        QMessageBox Msgbox(this);
        Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
        Msgbox.setText("保存成功！");
        Msgbox.exec();

        this->clear_selected_model();
        this->on_search_MODELCODE_LE_textChanged(this->ui->search_MODELCODE_LE->text());

        this->setEnabled(true);

    return;
}


/* set the selected item */
void MainWindow::on_search_model_result_Table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    const auto& table = this->ui->search_model_result_Table;
    table->selectRow(row);

    QList items = table->selectedItems();
    if(items.length() != this->num_search_model_result_table_columns) return;

    QString MODEL_CODE = items[0]->text(); // index 0 is the MODEL_CODE
    QString Container_ID = items[items.length()-1]->text();
    // set ID to empty if this model does not have a container
    if(Container_ID == none_CN || Container_ID == none_SPAN) Container_ID.clear();

    this->selected_model = inventory.get_Model(MODEL_CODE, Container_ID);

    this->show_selected_model();
}


/* the user is entering characters in the search Container LineEdit
   we want to search the container with the corresponding words.
*/
void MainWindow::on_search_CONTAINER_ID_LE_textChanged(const QString &new_str)
{
    this->setEnabled(false);

    auto table = this->ui->search_container_result_Table;
    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);
    if(new_str.isEmpty()){
        // if input is empty, empty the table and return
        this->setEnabled(true);
        this->ui->search_CONTAINER_ID_LE->setFocus();
        return;
    }

    QVector<ContainerPtr> containers; // will hold the containers that their ID starts with <new_str>
    inventory.searchContainer_starts_with(new_str, containers);

    // for each model, make a row for it
    for(UI row = 0; row < containers.size(); row++){
        const ContainerPtr container = containers[row];

        table->insertRow(table->rowCount());

        QVector<QString> items;
        container->searchResult(items);

        for( UI col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            table->setItem(row, col, tableWidgetItem);
        }
    }


    this->setEnabled(true);
    this->ui->search_CONTAINER_ID_LE->setFocus();
    return;
}


/* when the user click one cell, save the selected container and present its models in selected_container_table */
void MainWindow::on_search_container_result_Table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    const auto& table = this->ui->search_container_result_Table;
    table->selectRow(row);

    QList items = table->selectedItems();
    if(items.length() != this->num_search_container_result_table_columns) return;

    QString Container_ID = items[0]->text();
    // set ID to empty if this model does not have a container
    if(Container_ID == none_CN || Container_ID == none_SPAN) Container_ID.clear();

    this->selected_container = inventory.get_container(Container_ID);

    this->show_selected_container();
}


/* open the AddNewModelWindow for user to enter the information about the new model */
void MainWindow::on_start_add_model_btn_clicked()
{
    // init AddNewModelWindow
    QSharedPointer<AddNewModelWindow> w (new AddNewModelWindow(nullptr));
    this->AddNewModelWinPtr = w;
    // setting up the window
    w->setWindowTitle(AddNewModel_WinTitle);
    w->show();
    w->parentPtr = this;

    this->hide();
}


// 打开创建新清单的页面
void MainWindow::on_new_list_btn_clicked()
{
    // init CreateListWin
    QSharedPointer<CreateListWin> w (new CreateListWin(nullptr));
    this->CreateListWinPtr = w;
    // setting up the window
    w->setWindowTitle(CreateList_WinTitle);
    w->show();
    w->parentPtr = this;

    this->hide();
}

