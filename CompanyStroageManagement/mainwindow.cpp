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

const QString AddNewModelWin_Title = lan(AddNewModel_WinTitle_CN, AddNewModel_WinTitle_SPAN);
const QString CreateNewListWin_Title= lan(CreateList_WinTitle_CN, CreateList_WinTitle_SPAN);
const QString SearchListWin_Title= lan(Search_List_WinTitle_CN, Search_List_WinTitle_SPAN);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setLanguage();
    this->init();

    this->setWindow();

    // setting up the add new model window
    this->AddNewModelWinPtr.setWindowTitle(AddNewModelWin_Title);
    this->AddNewModelWinPtr.parentPtr = this;

    // setting up the create list window
    this->CreateListWinPtr.setWindowTitle(CreateNewListWin_Title);
    this->CreateListWinPtr.parentPtr = this;

    // setting up the search list window
    this->SearchListWinPtr.setWindowTitle(SearchListWin_Title);
    this->SearchListWinPtr.set_parentWin(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    // select the first tab initially
    this->ui->tabWidget->setCurrentIndex(0);

    // setting up tables
    auto model_table = ui->search_model_result_Table;
    model_table->setStyleSheet(table_stylesheet);

    auto container_table = ui->search_container_result_Table;
    container_table->setStyleSheet(table_stylesheet);

    auto selected_container_table = ui->selected_container_models_Table;
    selected_container_table->setStyleSheet(table_stylesheet);
}

void MainWindow::setWindow()
{
    QScreen *screen = QApplication::screens().at(0);
    QRect screenSize = screen->availableGeometry();

    int width = static_cast<int>(screenSize.width() * widthRatio);
    int height = static_cast<int>(screenSize.height() * heightRatio);

    this->resize(width, height);

    this->move(screenSize.width() / 2. - width / 2., screenSize.height() / 2. - height / 2.);
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
    const QStringList headers = {
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
    this->ui->search_model_result_Table->setHorizontalHeaderLabels(headers);

    this->ui->selected_model_GB->setTitle(lan("选中的货物", "las mercancías seleccionadas"));

    const QString none = lan("暂无", "ninguno");
    this->ui->selected_model_MODELCODE_label->setText(lan("货号", "MODELO"));
    this->ui->selected_model_MODELCODE_LE->setPlaceholderText(none);

    this->ui->selected_model_DESCRIPTION_CN_label->setText(lan("品名（中文）", "Nombre del producto (en chino)"));
    this->ui->selected_model_DESCRIPTION_CN_lineEdit->setPlaceholderText(none);

    this->ui->selected_model_DESCRIPTION_SPAN_label->setText(lan("品名（西语）", "Nombre del producto (en español)"));
    this->ui->selected_model_DESCRIPTION_SPAN_lineEdit->setPlaceholderText(none);

    this->ui->selected_model_NUM_INIT_PIECES_label->setText(lan("进货个数", "Núm. piezas adq."));
    this->ui->selected_model_NUM_PIECES_PER_BOX_label->setText(lan("每箱个数", "Número de piezas por caja"));

    this->ui->selected_model_PRIZE_label->setText(lan("单价", "Prec. por pieza"));
    this->ui->selected_model_NUM_SOLD_PIECES_label->setText(lan("已售个数", "Núm. piezas vend."));

    this->ui->selected_model_CONTAINER_label->setText(lan("集装箱号", "Número de contenedor"));
    this->ui->selected_model_CONTAINER_LE->setPlaceholderText(none);

    this->ui->delete_model_btn->setText(lan("删除货物", "Eliminar"));
    this->ui->update_selected_model_btn->setText(lan("更新该货物", "Actualizar la mercancía"));

    this->ui->search_CONTAINER_ID_label->setText(lan("集装箱号", "Número de contenedor"));
    this->ui->search_CONTAINER_ID_LE->setPlaceholderText(none);
    this->ui->search_container_result_GB->setTitle(lan("搜索结果", "resultados de búsqueda"));

    QStringList headers2 = {
        lan("集装箱号", "Número de contenedor"),
        lan("货物种类数量", "Núm. categorías merc."),
        lan("进货箱数", "Núm. cajas adq."),
        lan("剩余箱数", "Núm. cajas rest."),
        lan("进货个数", "Núm. piezas adq."),
        lan("剩余个数", "Núm. piezas rest."),
    };

    this->ui->search_container_result_Table->setHorizontalHeaderLabels(headers2);

    this->ui->selected_container_GB->setTitle(lan("该集装箱的货物", "la mercancía en este contenedor"));

    this->ui->selected_container_models_Table->setHorizontalHeaderLabels(headers);

    this->ui->start_add_model_btn->setText(lan("添加新货物", "añadir nueva mercancía"));

    this->ui->new_list_btn->setText(lan("建立新清单", "crear una nueva lista"));

    this->ui->search_past_list_btn->setText(lan("查询清单", "lista de consulta"));

    this->ui->save_inventory_2_new_file_btn->setText(lan("将库存保存至另一个文件", "guardar el inventario en otro archivo"));
    this->ui->read_inventory_from_new_file_btn->setText(lan("从另一个文件中读取库存", "leer el inventario desde otro archivo"));


    this->ui->save_lists_2_new_file_btn->setText(lan("将清单历史文件保存至另一个文件", "Guardar el archivo del historial de listas en otro archivo"));
    this->ui->read_lists_from_new_file_btn->setText(lan("从另一个文件中读取清单历史文件", "Lectura de un archivo de historial de listas desde otro archivo"));

    // action menu
    this->ui->FileMenu->setTitle(lan("文件", "Archivo"));
    this->ui->LanguageMenu->setTitle(lan("语言", "multilingüismo"));

    this->ui->action_Save_Inventory->setText(lan("保存库存文件", "Conservación de los archivos de inventario"));
    this->ui->action_Save_Lists->setText(lan("保存清单历史文件", "Guardar archivo de historial de listas"));
}


// show the selected model in the selected model group box
void MainWindow::show_selected_model()
{
    if(this->selected_model.isNull()) return;

    ui->selected_model_MODELCODE_LE->setText(this->selected_model->MODEL_CODE);
    ui->selected_model_DESCRIPTION_CN_lineEdit->setText(this->selected_model->DESCRIPTION_CN);
    ui->selected_model_DESCRIPTION_SPAN_lineEdit->setText(this->selected_model->DESCRIPTION_SPAN);
    ui->selected_model_NUM_INIT_PIECES_SB->setValue(this->selected_model->NUM_INIT_PIECES);
    ui->selected_model_NUM_SOLD_PIECES_SB->setValue(this->selected_model->NUM_SOLD_PIECES);
    ui->selected_model_NUM_PIECES_PER_BOX_SB->setValue(this->selected_model->NUM_PIECES_PER_BOX);
    ui->selected_model_PRIZE_SB->setValue(this->selected_model->PRIZE);

    if(this->selected_model->container.isNull()) ui->selected_model_CONTAINER_LE->setText("");
    else ui->selected_model_CONTAINER_LE->setText(this->selected_model->container->ID);
}


// reset this selected_model_GB, deselect model
void MainWindow::clear_selected_model()
{
    const QString empty;
    const double zero = 0.;
    ui->selected_model_MODELCODE_LE->setText(empty);
    ui->selected_model_DESCRIPTION_CN_lineEdit->setText(empty);
    ui->selected_model_DESCRIPTION_SPAN_lineEdit->setText(empty);
    ui->selected_model_NUM_INIT_PIECES_SB->setValue(zero);
    ui->selected_model_NUM_SOLD_PIECES_SB->setValue(zero);
    ui->selected_model_NUM_PIECES_PER_BOX_SB->setValue(zero);
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

    auto table = this->ui->selected_container_models_Table;
    QVector<ModelPtr> models;
    this->selected_container->models_Set2Vec(models, true); // sorted models

    // for each model, make a row for it
    unsigned long row = 0;
    for(const ModelPtr& model : models){
        table->insertRow(table->rowCount());

        QVector<QString> items;
        model->searchResult_Regular(items);

        for( unsigned long col = 0; col < items.size(); col++ ){
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
    auto table = this->ui->selected_container_models_Table;
    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);
}


/* 退出库存管理软件 */
void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox msg(this);
    msg.setText(lan(Are_You_Sure_to_Exit_CN, Are_You_Sure_to_Exit_SPAN));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::Yes) {
        WriteFile::save_settings_file();

        if(this->is_time_for_backup()){
            if( WriteFile::save_BackUp_files(false) )
                WriteFile::update_BackUpDate();
        }
        event->accept();
    }
    else {
        event->ignore();
    }
}


// emit this signal when this MainWindow is the foreground window
void MainWindow::set_as_front_window()
{
    this->show();
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
    for( unsigned long row = 0; row < models.size(); row++ ){
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

Ret:
    this->setEnabled(true);
    this->ui->search_MODELCODE_LE->setFocus();
}


/* update the model with the info entered */
void MainWindow::on_update_selected_model_btn_clicked()
{
    QMessageBox msg(this);
    msg.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 如果没有选中的model，需要提示用户
    if(this->selected_model.isNull()){
        msg.setText(lan("没有选中货物。", "No se ha seleccionado ninguna mercancía."));
        msg.exec();
        return;
    }

    // make sure user is indeed wanting to update the values
    msg.setText(lan(Are_You_Sure_to_Update_CN, Are_You_Sure_to_Update_SPAN));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::No) {
        this->setEnabled(true);
        return;
    }

    selected_model->DESCRIPTION_CN = ui->selected_model_DESCRIPTION_CN_lineEdit->text().replace("\n", "").trimmed(); // remove newline characters
    selected_model->DESCRIPTION_SPAN = ui->selected_model_DESCRIPTION_SPAN_lineEdit->text().replace("\n", "").trimmed(); // remove newline characters
    selected_model->NUM_INIT_PIECES = ui->selected_model_NUM_INIT_PIECES_SB->value();
    selected_model->NUM_SOLD_PIECES = ui->selected_model_NUM_SOLD_PIECES_SB->value();
    selected_model->PRIZE = ui->selected_model_PRIZE_SB->value();
    selected_model->NUM_PIECES_PER_BOX = ui->selected_model_NUM_PIECES_PER_BOX_SB->value();

    const QString container_ID = ui->selected_model_CONTAINER_LE->text().trimmed();

    if(container_ID.isEmpty()) // current container of selected model is empty, it must not exist
    {
        // check if the model has container previsously
        if(selected_model->container.isNull()) goto Finish; // if no, then we don't need to do anything
        else{ // the selected_model has container previously, we need to remove this model from its previous container
            this->selected_model->container->remove_model(this->selected_model);
            this->selected_model->container = nullptr;
        }
    }
    else{ // a container is entered, it may correspond to an existing container or not
        ContainerPtr prev_container = selected_model->container;
        // remove previous container first, it may be empty
        if(!prev_container.isNull()) // the selected_model has container previously, we need to remove this model from its previous container
        {
            prev_container->remove_model(this->selected_model);
            this->selected_model->container = nullptr;
        }

        // check if container_ID container exists in our record, if not, create one
        ContainerPtr cur_container = inventory.get_container(container_ID); // try to obtain the container obj with container_ID
        if(cur_container.isNull()){ // there is no existing container that is associated with this container_ID
            // create a new container with this ID
            cur_container = ContainerPtr (new Container(container_ID));
        }

        cur_container->add_model(this->selected_model); // add the selected model to the container
        selected_model->container = cur_container;
        inventory.add_Container(cur_container); // don't forget to add this container to the inventory

        goto Finish;
    }


Finish:
    QMessageBox Msgbox(this);
    Msgbox.setText(lan(SAVE_SUCCESS_MSG_CN, SAVE_SUCCESS_MSG_SPAN));
    Msgbox.exec();

    // update time
    selected_model->last_time_modified = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());

    this->update_GUI();

    // save the inventory and lists
    WriteFile::SaveInventoryAuto(false);
    WriteFile::Lists2txt(false);
}


/* set the selected item */
void MainWindow::on_search_model_result_Table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    const auto& table = this->ui->search_model_result_Table;
    table->selectRow(row);

    QList items = table->selectedItems();

    QString MODEL_CODE = items[0]->text().trimmed(); // index 0 is the MODEL_CODE
    QString Container_ID = items[1]->text().trimmed(); // index 1 is the container_ID
    // set ID to empty if this model does not have a container

    this->selected_model = inventory.get_Model(MODEL_CODE, Container_ID);

    this->show_selected_model();

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
    for( unsigned long row = 0; row < containers.size(); row++ ){
        const ContainerPtr container = containers[row];

        table->insertRow(table->rowCount());

        QVector<QString> items;
        container->searchResult(items);

        for( unsigned long col = 0; col < items.size(); col++ ){
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

    QString Container_ID = items[ 0 ]->text().trimmed(); // idx 1 is the container id

    this->selected_container = inventory.get_container(Container_ID);

    this->show_selected_container();
}


/* open the AddNewModelWindow for user to enter the information about the new model */
void MainWindow::on_start_add_model_btn_clicked()
{
    // init AddNewModelWindow
    this->AddNewModelWinPtr.set_GUI_Language();
    this->AddNewModelWinPtr.setWindow();


    this->AddNewModelWinPtr.show();

    this->hide();
}


// 打开创建新清单的页面
void MainWindow::on_new_list_btn_clicked()
{
    this->CreateListWinPtr.set_GUI_Language();
    this->CreateListWinPtr.setWindow();

    this->CreateListWinPtr.show();

    this->hide();
}


// 打开<Search_List_Win>
void MainWindow::on_search_past_list_btn_clicked()
{
    this->SearchListWinPtr.setWindow();
    this->SearchListWinPtr.set_GUI_Language();

    this->SearchListWinPtr.show();

    this->hide();
}


void MainWindow::on_delete_model_btn_clicked()
{
    QString prefix = lan(DELETE_MODEL_COMFIRMATION_MSG_PREFIX_CN, DELETE_MODEL_COMFIRMATION_MSG_SPAN);
    QString suffix = lan(DELETE_MODEL_COMFIRMATION_MSG_SUFFIX_CN, "?");

    int response = QMessageBox::No;
    QMessageBox delete_confirmation_msg(this);
    QMessageBox delete_success_msg(this);

    if(this->selected_model.isNull()){
        QMessageBox msg(this);
        msg.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 如果没有选中的model，需要提示用户
        msg.setText(lan("没有选中货物。", "No se ha seleccionado ninguna mercancía."));
        msg.exec();
        return;
    }

    // make sure user is indeed wanting to remove this model
    delete_confirmation_msg.setText(prefix + this->selected_model->MODEL_CODE + suffix);
    delete_confirmation_msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    delete_confirmation_msg.setDefaultButton(QMessageBox::No);
    delete_confirmation_msg.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    response = delete_confirmation_msg.exec();
    if (response == QMessageBox::No) goto ret;

    // delete this model from inventory
    inventory.remove_Model(this->selected_model);

    delete_success_msg.setText(lan(DELETE_SUCCESS_MSG_CN, DELETE_SUCCESS_MSG_SPAN));
    delete_success_msg.exec();

    this->update_GUI();

ret:
    // save the inventory and lists
    WriteFile::SaveInventoryAuto(false);
    WriteFile::Lists2txt(false);
}


// save inventory to a new file
void MainWindow::on_save_inventory_2_new_file_btn_clicked()
{
    // prompt the user to select folder and enter file name
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Excel Files (*.xlsx);; Text files (*.txt)");
    dialog.setDefaultSuffix("xlsx");
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    dialog.setDirectory(desktopPath);

    // Show the dialog and wait for the user's selection
    if (dialog.exec()) {
        QString filePath = dialog.selectedFiles().at(0);

        if(filePath.trimmed().isNull()) return;

        if( WriteFile::SaveInventoryAuto(filePath, true) )
        {
            QMessageBox msg(this);
            msg.setText(lan("保存成功", "Salvar con éxito"));
            msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            msg.exec();
            return;
        }
        else{
            QMessageBox msg(this);
            msg.setText(lan("保存失败", "no salvar"));
            msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            msg.exec();
            return;
        }
    }
}



// read inventory from a new file
void MainWindow::on_read_inventory_from_new_file_btn_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setNameFilter("Excel Files (*.xlsx);; Text files (*.txt)");
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    fileDialog.setDirectory(desktopPath);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    if (fileDialog.exec())
    {
        // Get the selected file
        QString fileName = fileDialog.selectedFiles().at(0);

        if(fileName.trimmed().isNull()){
            QMessageBox msg(this);
            msg.setText(lan(READ_FAIL_MSG_CN, READ_FAIL_MSG_CN));
            msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            return;
        }

        // clear the memory of inventory
        inventory.clear();

        // read the file
        if( !ReadFile::read_Inventory_File_Auto(fileName, true) ){
            QMessageBox* msgBox = new QMessageBox(this);
            msgBox->setAttribute(Qt::WA_DeleteOnClose);
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setText(lan(READ_FAIL_MSG_CN, READ_FAIL_MSG_CN));
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            msgBox->exec();
            return;
        }

        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(lan(READ_SUCCESS_MSG_CN, READ_SUCCESS_MSG_SPAN));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        msgBox->exec();
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

        QDateTime prev_DateTime = QDateTime::fromString(line, DateTimeFormat_for_backup_file);
        if(prev_DateTime.isValid()){
            QDateTime curDateTime = QDateTime::currentDateTime();
            unsigned int days = prev_DateTime.daysTo(curDateTime);
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


/* Save Lists file to a new path
 */
void MainWindow::on_save_lists_2_new_file_btn_clicked()
{
    // prompt the user to select folder and enter file name
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Text files (*.txt)");
    dialog.setDefaultSuffix("txt");
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    dialog.setDirectory(desktopPath);

    // Show the dialog and wait for the user's selection
    if (dialog.exec()) {
        QString filePath = dialog.selectedFiles().at(0);

        if(filePath.trimmed().isNull()) return;

        if( !WriteFile::Lists2txt(filePath, true) ){
            QMessageBox msg(this);
            msg.setText(lan("保存失败", "no salvar"));
            msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            return;
        }
    }
}


/* Read Lists file from a new path
 */
void MainWindow::on_read_lists_from_new_file_btn_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setNameFilter("Text files (*.txt)");
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setDefaultSuffix("txt");
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    fileDialog.setDirectory(desktopPath);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    if (fileDialog.exec())
    {
        // Get the selected file
        QString fileName = fileDialog.selectedFiles().at(0);

        if(fileName.trimmed().isNull()){
            QMessageBox msg(this);
            msg.setText(lan(READ_FAIL_MSG_CN, READ_FAIL_MSG_CN));
            msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            return;
        }

        // clear the memory of inventory
        lists.clear();

        // read the file
        if( !ReadFile::read_Lists_txt_File(fileName, true) ){
            QMessageBox* msgBox = new QMessageBox(this);
            msgBox->setAttribute(Qt::WA_DeleteOnClose);
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setText(lan(READ_FAIL_MSG_CN, READ_FAIL_MSG_CN));
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            msgBox->exec();
            return;
        }

        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(lan(READ_SUCCESS_MSG_CN, READ_SUCCESS_MSG_SPAN));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        msgBox->exec();
    }
}
