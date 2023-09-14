#include "Search_List_Win.h"
#include "GlobalVars.h"
#include "ui_Search_List_Win.h"

#include <QMessageBox>
#include <QCloseEvent>

Search_List_Win::Search_List_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search_List_Win)
{
    ui->setupUi(this);

    searched_lists_table = ui->searched_lists_table;
//    searched_lists_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    searched_lists_table->setStyleSheet(table_stylesheet);

    list_models_table = ui->list_models_table;
//    list_models_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    list_models_table->setStyleSheet(table_stylesheet);
}


Search_List_Win::~Search_List_Win()
{
    delete ui;
}


void Search_List_Win::set_parentWin(QWidget *win)
{
    this->parentWin = win;
}


void Search_List_Win::view_selected_list()
{
    if(this->selected_list.isNull()) return;

    this->ui->CLIENTE_LE->setText(selected_list->client_info.CLIENTE);
    this->ui->DOMICILIO_LE->setText(selected_list->client_info.DOMICILIO);
    this->ui->CIUDAD_LE->setText(selected_list->client_info.CIUDAD);

    this->ui->RFC_LE->setText(selected_list->client_info.RFC);
    this->ui->AGENTE_LE->setText(selected_list->client_info.AGENTE);
    this->ui->CONDICIONES_LE->setText(selected_list->client_info.CONDICIONES);
    this->ui->DISCOUNT_LE->setText(QString::number(selected_list->client_info.DISCOUNT));

    // show the models of this entry
    for(EntryPtr e : selected_list->itemList.entries){
        list_models_table->insertRow(list_models_table->rowCount());

        QVector<QString> items = e->view_values();

        for(UI col = 0; col < items.size(); col++){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );
            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            list_models_table->setItem(list_models_table->rowCount()-1, col, tableWidgetItem);
        }
    }
}


void Search_List_Win::reset_list_info()
{
    this->selected_list = nullptr;

    list_models_table->clearContents();
    list_models_table->setRowCount(0);

    this->ui->CLIENTE_LE->clear();
    this->ui->DOMICILIO_LE->clear();
    this->ui->CIUDAD_LE->clear();
    this->ui->RFC_LE->clear();
    this->ui->AGENTE_LE->clear();
    this->ui->CONDICIONES_LE->clear();
    this->ui->DISCOUNT_LE->clear();
}


void Search_List_Win::closeEvent (QCloseEvent *event)
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
        if(this->parentWin != nullptr) this->parentWin->show();
        event->accept();
    }
}


// user is typing the list id, and we use it to search lists
void Search_List_Win::on_list_id_2be_searched_LE_textChanged(const QString & list_id_prefix)
{
    // disable the window in case search takes time
    this->setDisabled(true);

    QString userInput = list_id_prefix.trimmed(); // remove useless empty spaces

    // clear content of <searched_lists_table>
    searched_lists_table->clearContents();
    searched_lists_table->setRowCount(0);

    QVector<ListPtr> candidates;
    lists.get_list(userInput, candidates, true);

    // for each list, make a row for it
    for(UI row = 0; row < candidates.size(); row++){
        const ListPtr list = candidates[row];

        searched_lists_table->insertRow(searched_lists_table->rowCount());

        QVector<QString> items = list->describe_this_list();

        for( UI col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            searched_lists_table->setItem(row, col, tableWidgetItem);
        }
    }

    // clear the selected list
    this->reset_list_info();

    this->setEnabled(true);
    this->ui->list_id_2be_searched_LE->setFocus();
}


void Search_List_Win::on_searched_lists_table_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    this->setDisabled(true);

    // clear content of <list_models_table>
    list_models_table->clearContents();
    list_models_table->setRowCount(0);

    searched_lists_table->selectRow(row);
    QList items = searched_lists_table->selectedItems();
    QString list_id = items[searched_lists_table_list_id_idx]->text();

    unsigned long int list_ID = list_id.toULong();

    ListPtr list = lists.get_list(list_ID);
    if(list.isNull()){
        goto Finish;
    }

    this->selected_list = list;

    // put the information of this list on UI
    this->view_selected_list();

Finish:
    this->setEnabled(true);
}

