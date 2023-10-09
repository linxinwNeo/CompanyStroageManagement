#include "mainwindow.h"
#include "CN_Strings.h"
#include "FileLoader/ReadFile.h"
#include "FileLoader/WriteFile.h"
#include "SpanStrings.h"
#include "qstandardpaths.h"
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

    this->setLanguage();
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
//    container_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    container_table->setStyleSheet(table_stylesheet);

    auto model_table = ui->search_model_result_Table;
//    model_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model_table->setStyleSheet(table_stylesheet);

    auto selected_container_table = ui->selected_container_Table;
//    selected_container_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    selected_container_table->setStyleSheet(table_stylesheet);
}


void MainWindow::clear_search_container_result_table()
{
    auto table = this->ui->search_container_result_Table;
    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);

    this->clear_selected_container_table();
    this->selected_container = nullptr;
}


// update the GUI
void MainWindow::update_GUI()
{
    this->on_search_MODELCODE_LE_textChanged(this->ui->search_MODELCODE_LE->text());
    this->on_search_CONTAINER_ID_LE_textChanged(this->ui->search_CONTAINER_ID_LE->text());
}


// set the language of mainWindow
void MainWindow::setLanguage()
{
    // change tab names
    this->ui->tabWidget->setTabText(0, lan("查询库存", "Consulta de inventario"));
    this->ui->tabWidget->setTabText(1, lan("查询集装箱", "Solicitud de información sobre contenedores"));
    this->ui->tabWidget->setTabText(2, lan("添加新货物", "Añadir nuevos productos"));
    this->ui->tabWidget->setTabText(3, lan("清单", "lista de artículos"));
    this->ui->tabWidget->setTabText(4, lan("保存或读取文件", "Guardar o leer archivos"));

    // change the first tab
    this->ui->search_MODELCODE_label->setText(lan("货号", "MODELO"));
    this->ui->search_MODELCODE_LE->setPlaceholderText(lan("在此输入需要查询的货号", "Introduzca aquí el número de artículo sobre el que desea realizar la consulta"));
    this->ui->search_model_result_GB->setTitle(lan("货物查询结果", "Resultados de la consulta sobre cargas"));
    QStringList headers = {
        lan("货号", "MODELO"),
        lan("品名(中文)", "DESCRIPTION(Chino)"),
        lan("品名(西语)", "DESCRIPTION(Español)"),
        lan("初始箱数", "Número inicial de cajas"),
        lan("每箱件数", "Piezas por caja"),
        lan("单价", "precio del artículo"),
        lan("已售箱数", "Número de cajas vendidas"),
        lan("已售件数", "Número de piezas vendidas"),
        lan("剩余箱数", "Número de cajas restantes"),
        lan("剩余件数", "Número de piezas restantes"),
        lan("集装箱号", "número de contenedor")
    };
    this->ui->search_model_result_Table->setHorizontalHeaderLabels(headers);

    this->ui->selected_model_GB->setTitle(lan("选中的货物", "las mercancías seleccionadas"));

    const QString none = lan("暂无", "ninguno");
    this->ui->selected_model_MODELCODE_label->setText(lan("货号", "MODELO"));
    this->ui->selected_model_MODELCODE_LE->setPlaceholderText(none);

    this->ui->selected_model_DESCRIPTION_CN_label->setText(lan("品名（中文）", "DESCRIPTION(Chino)"));
    this->ui->selected_model_DESCRIPTION_CN_LE->setPlaceholderText(none);

    this->ui->selected_model_DESCRIPTION_SPAN_label->setText(lan("品名（西语）", "DESCRIPTION(Español)"));
    this->ui->selected_model_DESCRIPTION_SPAN_LE->setPlaceholderText(none);

    this->ui->selected_model_NUM_INIT_BOXES_label->setText(lan("初始箱数", "Número inicial de cajas"));
    this->ui->selected_model_NUM_ITEMS_PER_BOX_label->setText(lan("每箱件数", "Piezas por caja"));

    this->ui->selected_model_PRIZE_label->setText(lan("单价", "precio del artículo"));
    this->ui->selected_model_NUM_SOLD_BOXES_label->setText(lan("已售箱数", "Número de cajas vendidas"));

    this->ui->selected_model_CONTAINER_label->setText(lan("集装箱号", "número de contenedor"));
    this->ui->selected_model_CONTAINER_LE->setText(none);

    this->ui->delete_model_btn->setText(lan("删除货物", "eliminar"));
    this->ui->update_selected_model_btn->setText(lan("更新货物", "actualizar productos"));

    this->ui->search_CONTAINER_ID_label->setText(lan("集装箱号", "número de contenedor"));
    this->ui->search_CONTAINER_ID_LE->setPlaceholderText(none);
    this->ui->search_container_result_GB->setTitle(lan("搜索结果", "resultados de búsqueda"));

    QStringList headers2 = {
        lan("集装箱号", "número de contenedor"),
        lan("货物种类数量", "cantidad de tipos de mercancías"),
        lan("初始总箱数（箱）", "número total inicial de cajas (cajas)"),
        lan("货物剩余数量（箱）", "cantidad restante de mercancía (cajas)"),
        lan("货物初始数量（件）", "cantidad inicial de mercancía (unidades)"),
        lan("货物剩余数量（件）", "cantidad restante de mercancía (unidades)"),
    };

    this->ui->search_container_result_Table->setHorizontalHeaderLabels(headers2);
   //TODO

    this->ui->selected_container_GB->setTitle(lan("该集装箱的货物", "la mercancía en este contenedor"));

    this->ui->selected_container_Table->setHorizontalHeaderLabels(headers);

    this->ui->start_add_model_btn->setText(lan("添加新货物", "añadir nueva mercancía"));

    this->ui->new_list_btn->setText(lan("建立新清单", "crear una nueva lista"));

    this->ui->search_past_list_btn->setText(lan("查询清单", "lista de consulta"));

    this->ui->save2_new_file_btn->setText(lan("将库存保存至另一个文件", "guardar el inventario en otro archivo"));
    this->ui->read_from_new_file_btn->setText(lan("从另一个文件中读取库存", "leer el inventario desde otro archivo"));
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

    if(this->selected_model->container.isNull()) ui->selected_model_CONTAINER_LE->setText("");
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
    msg.setText(lan(Are_You_Sure_to_Exit_CN, Are_You_Sure_to_Exit_SPAN));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::Yes) {
        WriteFile wf;
        wf.save_settings_file();

        if(this->is_time_for_backup()){
            if(wf.save_BackUp_files()) wf.update_BackUpDate();
        }
        event->accept();
    }
    else {
        event->ignore();
    }
}


/* the user is typing in MODELCODE, we need to search the models starting with the same string and display them in
 *  the searchResult table */
void MainWindow::on_search_MODELCODE_LE_textChanged(const QString& new_str)
{
    this->setDisabled(true);

    this->clear_selected_model();

    QString userInput = new_str.trimmed(); // remove useless empty spaces
    QVector<ModelPtr> models; // will hold the models that has MODELCODE starts with new_str

    auto table = this->ui->search_model_result_Table;
    this->clear_search_model_result_table();

    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);
    if(userInput.isEmpty()){
        // if input is empty, empty the table and return
        goto Ret;
    }

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

Ret:
    this->setEnabled(true);
    this->ui->search_MODELCODE_LE->setFocus();
}


/* update the model with the info entered */
void MainWindow::on_update_selected_model_btn_clicked()
{
    this->setEnabled(false);

    // make sure user is indeed wanting to update the values
    QMessageBox msg(this);
    msg.setText(lan(Are_You_Sure_to_Update_CN, Are_You_Sure_to_Update_SPAN));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::No);
    msg.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::No) {
        this->setEnabled(true);
        return;
    }

    selected_model->DESCRIPTION_CN = ui->selected_model_DESCRIPTION_CN_LE->text().trimmed();
    selected_model->DESCRIPTION_SPAN = ui->selected_model_DESCRIPTION_SPAN_LE->text().trimmed();
    selected_model->NUM_INIT_BOXES = ui->selected_model_NUM_INIT_BOXES_SB->value();
    selected_model->NUM_SOLD_BOXES = ui->selected_model_NUM_SOLD_BOXES_SB->value();
    selected_model->PRIZE = ui->selected_model_PRIZE_SB->value();
    selected_model->NUM_ITEMS_PER_BOX = ui->selected_model_NUM_ITEMS_PER_BOX_SB->value();

    selected_model->NUM_LEFT_BOXES = selected_model->NUM_INIT_BOXES - selected_model->NUM_SOLD_BOXES;
    selected_model->NUM_LEFT_ITEMS = selected_model->NUM_INIT_BOXES * selected_model->NUM_ITEMS_PER_BOX;

    const QString container_ID = ui->selected_model_CONTAINER_LE->text().trimmed();

    if(container_ID.isEmpty()) // current container of selected model does not exist
    {
        // check if the model has container previsously
        if(selected_model->container.isNull()) goto Finish; // if no, then we don't need to do anything
        else{ // the selected_model has container previously, we need to remove this model from its previous container
            this->selected_model->container->remove_model(this->selected_model);
            this->selected_model->container = nullptr;
        }
    }
    else{ // the container exists
        // check if current container exists, if not, create one
        ContainerPtr prev_container = selected_model->container;
        ContainerPtr cur_container = inventory.get_container(container_ID);
        if(cur_container.isNull()){ // if no such container exists, create one
            cur_container = ContainerPtr (new Container(container_ID));
            cur_container->add_model(this->selected_model); // add the selected model to the container
            selected_model->container = cur_container;
        }

        // check if the container ID is the same as before
        if(container_ID == prev_container->ID){
            goto Finish;
        }
        else{
            // the container for the model has been modified

            // remove selected_model from the previous container
            prev_container->remove_model(selected_model);
            selected_model->container = cur_container;
        }
    }


Finish:
    QMessageBox Msgbox(this);
    Msgbox.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox.setText(lan(SAVE_SUCCESS_MSG_CN, SAVE_SUCCESS_MSG_SPAN));
    Msgbox.exec();

    this->update_GUI();

    this->setEnabled(true);
}


/* set the selected item */
void MainWindow::on_search_model_result_Table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    this->setEnabled(false);

    const auto& table = this->ui->search_model_result_Table;
    table->selectRow(row);

    QList items = table->selectedItems();

    QString MODEL_CODE = items[search_model_result_table_MODELCODE_idx]->text().trimmed(); // index 0 is the MODEL_CODE
    QString Container_ID = items[search_model_result_table_ContainerID_idx]->text().trimmed();
    // set ID to empty if this model does not have a container

    this->selected_model = inventory.get_Model(MODEL_CODE, Container_ID);

    this->show_selected_model();

    this->setEnabled(true);
}


/* the user is entering characters in the search Container LineEdit
   we want to search the container with the corresponding words.
*/
void MainWindow::on_search_CONTAINER_ID_LE_textChanged(const QString &new_str)
{
    this->setEnabled(false);

    this->clear_selected_container_table();

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

    QString Container_ID = items[ search_container_result_table_ContainerID_idx ]->text().trimmed();

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
    QString title = lan(AddNewModel_WinTitle_CN, AddNewModel_WinTitle_SPAN);

    w->setWindowTitle(title);
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
    QString title = lan(CreateList_WinTitle_CN, CreateList_WinTitle_SPAN);

    w->setWindowTitle(title);
    w->parentPtr = this;
    w->show();

    this->hide();
}


// 打开<Search_List_Win>
void MainWindow::on_search_past_list_btn_clicked()
{
    QSharedPointer<Search_List_Win> w (new Search_List_Win(nullptr));
    this->SearchListWinPtr = w;

    QString title = lan(Search_List_WinTitle_CN, Search_List_WinTitle_SPAN);

    w->setWindowTitle(title);
    w->set_parentWin(this);

    w->show();

    this->hide();
}


void MainWindow::on_delete_model_btn_clicked()
{
    this->setEnabled(false);

    QString prefix = lan(DELETE_MODEL_COMFIRMATION_MSG_PREFIX_CN, DELETE_MODEL_COMFIRMATION_MSG_SPAN);
    QString suffix = lan(DELETE_MODEL_COMFIRMATION_MSG_SUFFIX_CN, "?");
    QString msg = lan(DELETE_SUCCESS_MSG_CN, DELETE_SUCCESS_MSG_SPAN);

    int response = QMessageBox::No;
    QMessageBox delete_confirmation_msg(this);
    QMessageBox delete_success_msg(this);

    if(this->selected_model.isNull()) goto ret;

    // make sure user is indeed wanting to remove this model
    delete_confirmation_msg.setText(prefix + this->selected_model->MODEL_CODE + suffix);
    delete_confirmation_msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    delete_confirmation_msg.setDefaultButton(QMessageBox::No);
    delete_confirmation_msg.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    delete_confirmation_msg.setStyleSheet("QLabel{min-width: 400px; min-height: 50px;}");

    response = delete_confirmation_msg.exec();
    if (response == QMessageBox::No) goto ret;

    // delete this model from inventory
    inventory.remove_Model(this->selected_model);

    delete_success_msg.setText(DELETE_SUCCESS_MSG_CN);
    delete_success_msg.exec();

    this->update_GUI();

ret:
    this->setEnabled(true);
}


// save inventory to a new file
void MainWindow::on_save2_new_file_btn_clicked()
{
    // prompt the user to select folder and enter file name
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Excel Files (*.xlsx)");
    dialog.setDefaultSuffix("xlsx");
    dialog.selectFile(Inventory_FNAME_xlsx);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    dialog.setDirectory(desktopPath);

    // Show the dialog and wait for the user's selection
    if (dialog.exec()) {
        QString filePath = dialog.selectedFiles().first();

        if(filePath.trimmed().isNull()) return;

        WriteFile WF;
        WF.Inventory2Xlsx(filePath);
    }
}



// read inventory from a new file
void MainWindow::on_read_from_new_file_btn_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setNameFilter("Excel Files (*.xlsx)");
    fileDialog.setDefaultSuffix("xlsx");
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    fileDialog.setDirectory(desktopPath);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.selectFile(Inventory_FNAME_xlsx);

    if (fileDialog.exec())
    {
        // Get the selected file
        QString fileName = fileDialog.selectedFiles().first();

        if(fileName.trimmed().isNull()){
            QMessageBox msg(this);
            msg.setText(lan(READ_FAIL_MSG_CN, READ_FAIL_MSG_CN));
            msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
            return;
        }

        // clear the memory of inventory
        inventory.clear();

        // read the file
        ReadFile RF;
        RF.read_Inventory_xlsx_File(fileName);

        QMessageBox msg(this);
        msg.setText(lan(READ_SUCCESS_MSG_CN, READ_SUCCESS_MSG_SPAN));
        msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    }
}


// check if it has been a week since last back up
bool MainWindow::is_time_for_backup() const
{
    // read datetime for last backup
    QFile file(BackUP_FileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        QString line = stream.readLine();

        QDateTime prev_DateTime = QDateTime::fromString(line, DateTimeFormat);
        if(prev_DateTime.isValid()){
            QDateTime curDateTime = QDateTime::currentDateTime();
            int days = prev_DateTime.daysTo(curDateTime);
            if(days > backup_every_n_days){
                return true;
            }
            else
                return false;
        }
        else return true;

    }
    else{ // no backup history, save
        return true;
    }
}


// change language to Spanish
void MainWindow::on_actionSpanish_triggered()
{
    this->setDisabled(true);

    language_option = 1;
    this->setLanguage();

    this->setEnabled(true);
}


// change language to Chinese
void MainWindow::on_actionChinese_triggered()
{
    this->setDisabled(true);

    language_option = 0;
    this->setLanguage();

    this->setEnabled(true);
}

