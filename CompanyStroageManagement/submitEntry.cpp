#include "submitEntry.h"
#include "ui_submitEntry.h"

submitEntry::submitEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::submitEntry)
{
    ui->setupUi(this);
}

submitEntry::~submitEntry()
{
    delete ui;
}
