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
    delete ui;
}

void MainWindow::init()
{
    // setting up tables
    auto container_table = ui->search_container_result_Table;
    container_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    container_table->setStyleSheet(table_stylesheet);

    auto model_table = ui->search_model_result_Table;
    model_table->setStyleSheet(table_stylesheet);

    auto selected_container_table = ui->selected_container_Table;
    selected_container_table->setStyleSheet(table_stylesheet);

    // read inventory.txt file
    ReadFile read_file;
    read_file.read_Models(DB_FNAME); // build the inventory
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
void MainWindow::on_search_MODELCODE_LE_textChanged(QString new_str)
{
    this->setEnabled(false);

    auto table = this->ui->search_model_result_Table;

    table->clearContents(); // clear the table contents but columns are reserved
    table->setRowCount(0);
    if(new_str.isEmpty()){
        // if input is empty, empty the table
        this->setEnabled(true);
        this->ui->search_MODELCODE_LE->setFocus();
        return;
    }

    QVector<ModelPtr> models; // will hold the models that has MODELCODE starts with new_str
    inventory.searchModel_starts_with(new_str, models);

    // for each model, make a row for it
    for(UI row = 0; row < models.size(); row++){
        const ModelPtr model = models[row];

        table->insertRow(table->rowCount());

        QVector<QString> items;
        model->searchResult(items);

        for( UI col = 0; col < items.size(); col++ ){
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
            tableWidgetItem->setText( items[col] );
            table->setItem(row, col, tableWidgetItem);
        }
    }

    this->setEnabled(true);
    this->ui->search_MODELCODE_LE->setFocus();
    return;
}


/* when the user double clicked a row, we want to investiage the corresponding model */
void MainWindow::on_search_model_result_Table_cellDoubleClicked(int row, int column)
{
    qDebug() << row << column;
}

