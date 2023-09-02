#include "Search_List_Win.h"
#include "ui_Search_List_Win.h"

Search_List_Win::Search_List_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search_List_Win)
{
    ui->setupUi(this);
}

Search_List_Win::~Search_List_Win()
{
    delete ui;
}
