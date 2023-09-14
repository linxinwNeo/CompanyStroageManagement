#include "Adjust_List_Item_Win.h"
#include "CreateListWin.h"
#include "qevent.h"
#include "ui_Adjust_List_Item_Win.h"

Adjust_List_Item_Win::Adjust_List_Item_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Adjust_List_Item_Win)
{
    ui->setupUi(this);

    this->model_2be_adjusted = nullptr;
}


Adjust_List_Item_Win::~Adjust_List_Item_Win()
{
    delete ui;
}


void Adjust_List_Item_Win::set_model_and_entry(ModelPtr model, EntryPtr entry, const unsigned int entry_idx)
{
    this->model_2be_adjusted = model;
    this->entry_2be_adjusted = entry;
    this->entry_idx = entry_idx;
}


void Adjust_List_Item_Win::set_GUI()
{
    if(this->model_2be_adjusted.isNull() || this->entry_2be_adjusted.isNull()) return;

    const double NUM_BOXES = this->entry_2be_adjusted->CAJA;

    this->ui->NUM_BOXES_LE->setText(QString::number(NUM_BOXES, 'f', 2));

    this->ui->NUM_ITEMS_SB->setMaximum(model_2be_adjusted->NUM_LEFT_ITEMS);
    this->ui->NUM_ITEMS_LE->setText(QString::number(model_2be_adjusted->NUM_LEFT_ITEMS));
    this->ui->NUM_ITEMS_SB->setValue(entry_2be_adjusted->CANTIDAD);


    this->ui->NUM_ITEMS_PER_BOX_LE->setText(QString::number(model_2be_adjusted->NUM_ITEMS_PER_BOX));

    this->ui->MODELCODE_LE->setText(model_2be_adjusted->MODEL_CODE);

    this->ui->DESCRIPTION_CN_TE->setText(model_2be_adjusted->DESCRIPTION_CN);
    this->ui->DESCRIPTION_SPAN_TE->setText(model_2be_adjusted->DESCRIPTION_SPAN);

    this->ui->PREZE_LE->setText(QString::number(model_2be_adjusted->PRIZE, 'f', 2));

    this->ui->TOTAL_LE->setText(QString::number(model_2be_adjusted->PRIZE * entry_2be_adjusted->CANTIDAD, 'f', 2));
}


void Adjust_List_Item_Win::closeEvent(QCloseEvent *event)
{
    if(this->parent_win != nullptr){
        this->parent_win->update_added_models_table();
        this->parent_win->setEnabled(true);
    }

    event->accept();
}


// update the corresponding row in <added_models_table>
void Adjust_List_Item_Win::on_finish_btn_clicked()
{
    if(!model_2be_adjusted || !entry_2be_adjusted) this->close();

    // if the item number is 0, we remove the entry from this list entries in the parent Window
    if(this->ui->NUM_BOXES_LE->text().toDouble() == 0 || this->ui->NUM_ITEMS_SB->value() == 0){
        this->parent_win->remove_entry(this->entry_idx);
    }
    else{
        // we only need to update three information: num of boxes, num of items, total of the entry
        entry_2be_adjusted->CAJA = this->ui->NUM_BOXES_LE->text().toDouble();
        entry_2be_adjusted->CANTIDAD = this->ui->NUM_ITEMS_SB->value();
        entry_2be_adjusted->IMPORTE = this->ui->TOTAL_LE->text().toDouble();
    }

    // we need to update the entry of corresponding model
    this->close();
}


void Adjust_List_Item_Win::on_NUM_ITEMS_SB_valueChanged(int cur_num_items)
{
    double num_boxes = cur_num_items / (double) this->model_2be_adjusted->NUM_ITEMS_PER_BOX;

    this->ui->NUM_BOXES_LE->setText(QString::number(num_boxes, 'f', 2));

    const double total = cur_num_items * model_2be_adjusted->PRIZE;
    this->ui->TOTAL_LE->setText(QString::number(total, 'f', 2));
}

