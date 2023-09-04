#include "Adjust_List_Item_Win.h"
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


void Adjust_List_Item_Win::set_init_UI_values(ModelPtr model, const unsigned long int NUM_ITEMS)
{
    this->model_2be_adjusted = model;

    const double NUM_BOXES = ((double) NUM_ITEMS) / model->NUM_ITEMS_PER_BOX;

    this->ui->NUM_BOXES_LE->setText(QString::number(NUM_BOXES, 'f', 2));

    this->ui->NUM_ITEMS_SB->setMaximum(model->NUM_LEFT_ITEMS);
    this->ui->NUM_ITEMS_LE->setText(QString::number(model->NUM_LEFT_ITEMS, 'f', 2));
    this->ui->NUM_ITEMS_SB->setValue(NUM_ITEMS);


    this->ui->NUM_ITEMS_PER_BOX_LE->setText(QString::number(model->NUM_ITEMS_PER_BOX));

    this->ui->MODELCODE_LE->setText(model->MODEL_CODE);

    this->ui->DESCRIPTION_CN_TE->setText(model->DESCRIPTION_CN);
    this->ui->DESCRIPTION_SPAN_TE->setText(model->DESCRIPTION_SPAN);

    this->ui->PREZE_LE->setText(QString::number(model->PRIZE, 'f', 2));

    this->ui->TOTAL_LE->setText(QString::number(model->PRIZE * NUM_ITEMS, 'f', 2));
}


void Adjust_List_Item_Win::closeEvent(QCloseEvent *event)
{
    if(this->parent_win != nullptr) this->parent_win->setEnabled(true);

    event->accept();
}


// update the corresponding row in <added_models_table>
void Adjust_List_Item_Win::on_finish_btn_clicked()
{
    if(!this->added_models_table) this->close();

    QList<QTableWidgetItem*> selected_items = this->added_models_table->selectedItems();

    // if the item number is 0, we remove the target row in the parentWin's <added_models_table>
    if(this->ui->NUM_BOXES_LE->text().toDouble() == 0 || this->ui->NUM_ITEMS_SB->value() == 0){
        unsigned int row_idx = selected_items[0]->row();
        this->added_models_table->removeRow(row_idx);
    }

    // we only need to update three information: num of boxes, num of items, total
    selected_items[0]->setText(this->ui->NUM_BOXES_LE->text());
    selected_items[1]->setText(QString::number(this->ui->NUM_ITEMS_SB->value()));
    selected_items[7]->setText(this->ui->TOTAL_LE->text());

    this->close();
}


void Adjust_List_Item_Win::on_NUM_ITEMS_SB_valueChanged(int cur_num_items)
{
    if(cur_num_items > this->model_2be_adjusted->NUM_LEFT_ITEMS){
        cur_num_items = this->model_2be_adjusted->NUM_LEFT_ITEMS;
        this->ui->NUM_ITEMS_SB->setValue(cur_num_items);
    }

    double num_boxes = cur_num_items / (double) this->model_2be_adjusted->NUM_ITEMS_PER_BOX;

    this->ui->NUM_BOXES_LE->setText(QString::number(num_boxes, 'f', 2));

    const double total = cur_num_items * model_2be_adjusted->PRIZE;
    this->ui->TOTAL_LE->setText(QString::number(total, 'f', 2));
}

