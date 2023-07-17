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

    // setting up the table
    auto table = ui->searchResult_Table;
    table->setRowCount(0);
    table->setColumnCount(8);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // read inventory.txt file
    ReadFile tet;
    tet.read_Models(DB_FNAME); // build the inventory
}


MainWindow::~MainWindow()
{
    delete ui;
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

    this->ui->searchResult_Table->clearContents();


    this->setEnabled(true);
    return;
}

