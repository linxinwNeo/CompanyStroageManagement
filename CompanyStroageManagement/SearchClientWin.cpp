#include "SearchClientWin.h"
#include "ui_SearchClientWin.h"

#include "GlobalVars.h"
#include <QScreen>
#include <QMessageBox>
#include <Others/write_error_file.h>

SearchClientWin::SearchClientWin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchClientWin)
{
    ui->setupUi(this);
}

SearchClientWin::~SearchClientWin()
{
    delete ui;
}

void SearchClientWin::set_GUI_Language()
{
    const QString enterHere = lan("在此输入", "Entra aquí");

    this->ui->label_ClientID->setText(lan("客户号码", "Número de cliente"));
    this->ui->lineEdit_ClientID->setPlaceholderText(enterHere);

    this->ui->label_ClientName->setText(lan("客户名字", "CLIENTE"));
    this->ui->lineEdit_ClientName->setPlaceholderText(enterHere);

    const QString search = lan("搜索", "Buscar");
    this->ui->pushButton_SearchByIDPrefix->setText(search);
    this->ui->pushButton_SearchByNamePrefix->setText(search);

    this->ui->pushButton_deleteSelectedClient->setText(lan("删除", "Borrar"));

    this->ui->tableWidget_searchedClients->setHorizontalHeaderLabels(GlobalVars::table_headers_client());
}

void SearchClientWin::setWindow()
{
    QScreen *screen = QApplication::screens().at(0);
    QRect screenSize = screen->availableGeometry();

    int width = static_cast<int>(screenSize.width() * widthRatio);
    int height = static_cast<int>(screenSize.height() * heightRatio);

    this->resize(width, height);

    this->move(screenSize.width() / 2 - width / 2, screenSize.height() / 2 - height / 2);
}

void SearchClientWin::clear_table()
{
    QTableWidget* table = this->ui->tableWidget_searchedClients;

    // clear previous table contents
    table->clearContents();
    table->setRowCount(0);
}


void SearchClientWin::on_pushButton_SearchByIDPrefix_clicked()
{
    this->setDisabled(true);

    this->ui->lineEdit_ClientName->clear();

    QTableWidget* table = this->ui->tableWidget_searchedClients;

    // clear previous table contents
    clear_table();

    QVector< QSharedPointer<Client> > candidate_clients;

    const QString id_tobe_searched = this->ui->lineEdit_ClientID->text().trimmed();
    if(id_tobe_searched.isEmpty()) goto Finish;


    clientManager.get_clients_by_IDPrefix(id_tobe_searched, candidate_clients);

    if(candidate_clients.size() == 0) goto Finish;

    for(unsigned long row = 0; row < candidate_clients.size(); row++){
        QSharedPointer<Client>& client = candidate_clients[row];

        table->insertRow(table->rowCount());
        QVector<QString> items;
        client->searchResult(items);

        for( unsigned long col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            table->setItem(row, col, tableWidgetItem);
        }
    }


    goto Finish;

Finish:
    this->setDisabled(false);
    return;
}


void SearchClientWin::on_pushButton_SearchByNamePrefix_clicked()
{
    this->setDisabled(true);

    this->ui->lineEdit_ClientID->clear();

    QTableWidget* table = this->ui->tableWidget_searchedClients;

    // clear previous table contents
    this->clear_table();

    QVector< QSharedPointer<Client> > candidate_clients;

    const QString name_tobe_searched = this->ui->lineEdit_ClientName->text().trimmed();
    if(name_tobe_searched.isEmpty()) goto Finish;


    clientManager.get_clients_by_NamePrefix(name_tobe_searched, candidate_clients);

    if(candidate_clients.size() == 0) goto Finish;

    for(unsigned long row = 0; row < candidate_clients.size(); row++){
        QSharedPointer<Client>& client = candidate_clients[row];

        table->insertRow(table->rowCount());
        QVector<QString> items;
        client->searchResult(items);

        for( unsigned long col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );

            tableWidgetItem->setTextAlignment(Qt::AlignVCenter);

            table->setItem(row, col, tableWidgetItem);
        }
    }


    goto Finish;

Finish:
    this->setDisabled(false);
    return;
}



void SearchClientWin::on_pushButton_deleteSelectedClient_clicked()
{
    QTableWidget* table = this->ui->tableWidget_searchedClients;
    auto selectedItems = table->selectedItems();
    if(selectedItems.size() == 0) return;

    // figure out which client is selected
    const QString selected_client_id = selectedItems[0]->text();
    if(clientManager.delete_client(selected_client_id)){
        // 删除table的row
        table->removeRow(selectedItems[0]->row());

        QMessageBox msg;
        msg.setText(lan("删除客户成功！", "¡Elimine el éxito del cliente!"));
        msg.exec();
        return;
    }
    else{
        QMessageBox msg;
        msg.setText(lan("删除客户失败！", "¡No se pudo eliminar el cliente!"));
        msg.exec();
        write_error_file(QString("SearchClientWin::on_pushButton_deleteSelectedClient_clicked: ") +
                         "由于ClientManager::delete_client返回了false，所以客户删除失败了。");
        return;
    }
}

