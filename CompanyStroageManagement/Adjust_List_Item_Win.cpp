#include "Adjust_List_Item_Win.h"
#include "ui_Adjust_List_Item_Win.h"

Adjust_List_Item_Win::Adjust_List_Item_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Adjust_List_Item_Win)
{
    ui->setupUi(this);
    this->parent_win = parent;
}


Adjust_List_Item_Win::~Adjust_List_Item_Win()
{
    delete ui;
}

void Adjust_List_Item_Win::set_values(const double MAX_NUM_BOXES,
                                      const long unsigned int MAX_NUM_ITEMS,
                                      const long unsigned int NUM_ITEMS_PER_BOX,
                                      const QString MODEL_CODE, const QString Description,
                                      const double PRIZE, const double TOTAL)
{
    this->MAX_NUM_BOXES = MAX_NUM_BOXES;
    this->MAX_NUM_ITEMS = MAX_NUM_ITEMS;
    this->NUM_ITEMS_PER_BOX = NUM_ITEMS_PER_BOX;
    this->MODEL_CODE = MODEL_CODE;
    this->DESCRIPTION = Description;
    this->PRIZE = PRIZE;
    this->TOTAL = TOTAL;

    this->ui->NUM_BOXES_LE->setText(QString::number(this->MAX_NUM_BOXES));
    this->ui->NUM_BOXES_SB->setValue(this->MAX_NUM_BOXES);
    this->ui->NUM_BOXES_SB->setMaximum(this->NUM_BOXES);

    this->ui->NUM_ITEMS_LE->setText(QString::number(this->MAX_NUM_ITEMS));
    this->ui->NUM_ITEMS_SB->setValue(this->MAX_NUM_ITEMS);
    this->ui->NUM_ITEMS_SB->setMaximum(this->NUM_BOXES);

    this->ui->NUM_ITEMS_PER_BOX_LE->setText(QString::number(this->NUM_ITEMS_PER_BOX));

    this->ui->MODELCODE_LE->setText(this->MODEL_CODE);

    this->ui->DESCRIPTION_LE->setText(this->DESCRIPTION);

    this->ui->PREZE_LE->setText(QString::number(this->PRIZE));

    this->ui->TOTAL_LE->setText(QString::number(this->TOTAL));
}


// update the corresponding row in <added_models_table>
//TODO
void Adjust_List_Item_Win::on_finish_btn_clicked()
{

}

