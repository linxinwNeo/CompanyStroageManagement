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
        model->searchResult(items);

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

    //设置表格 style
    searched_models_table->setStyleSheet(table_stylesheet);
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


/* 用户点击了一个cell，我们要选中整行，然后通过modelCODE寻找reference， 然后加入这个到added_models_table
 * 要注意的是，如果要加入的这个model已经在<added_models_table>了，我们可以什么都不做。 */
void CreateListWin::on_searched_models_table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    const auto& table = this->searched_models_table;
    table->selectRow(row);

    QList items = table->selectedItems();
    if(items.length() != this->NUM_SEARCHED_MODELS_TABLE_COLS) return;

    QString MODEL_CODE = items[0]->text(); // index 0 is the MODEL_CODE
    QString Container_ID = items[items.length()-1]->text();
    // set ID to empty if this model does not have a container
    if(Container_ID == none_CN || Container_ID == none_SPAN) Container_ID.clear();

    this->selected_model_in_search_table = inventory.get_Model(MODEL_CODE, Container_ID);

    // check if this model is in <added_models_table> already
    //TODO
}

