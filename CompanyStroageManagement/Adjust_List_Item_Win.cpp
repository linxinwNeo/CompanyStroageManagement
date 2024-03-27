#include "Adjust_List_Item_Win.h"
#include "CreateListWin.h"
#include "GlobalVars.h"
#include "qevent.h"
#include "ui_Adjust_List_Item_Win.h"

Adjust_List_Item_Win::Adjust_List_Item_Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Adjust_List_Item_Win)
{
    ui->setupUi(this);

    this->model_2be_adjusted = nullptr;
    this->setWindow();

}


// set language according to the current language
void Adjust_List_Item_Win::setLanguage()
{
    //const QString none = lan("暂无", "ninguno");

    this->ui->adjust_list_item_GB->setTitle(lan("调整出售货物的数量", "Ajuste de la cantidad de bienes vendidos"));
    this->ui->finish_btn->setText(lan("完成", "Completar"));

    this->ui->NUM_BOXES_Label->setText(lan("箱数", "Número de cajas"));
    this->ui->NUM_PIECES_Label->setText(lan("个"Número de cajas"));
}


void Adjust_List_Item_Win::setWindow()
{
    QScreen *screen = QApplication::screens().at(0);
    QRect screenSize = screen->availableGeometry();

    int width = static_cast<int>(screenSize.width() * widthRatio);
    int height = static_cast<int>(screenSize.height() * heightRatio);

    this->resize(width, height);

    this->move(screenSize.width() / 2. - width / 2., screenSize.height() / 2. - height / 2.);
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


void Adjust_List_Item_Win::set_GUI_values()
{
    if(this->model_2be_adjusted.isNull() || this->entry_2be_adjusted.isNull()) return;

    unsigned long CURR_NUM_PIECES = this->entry_2be_adjusted->NUM_PIECES;
    // make sure current num of pieces is smaller than the left pieces
    if(CURR_NUM_PIECES > this->model_2be_adjusted->NUM_LEFT_PIECES){
        CURR_NUM_PIECES = this->model_2be_adjusted->NUM_LEFT_PIECES;
    }

    this->ui->MAX_NUM_BOXES_LineEdit->setText(QString::number(this->model_2be_adjusted->num_left_boxes(), 'f', 2));

    this->ui->NUM_BOXES_DoubleSpinBox->setMaximum(this->model_2be_adjusted->num_left_boxes());
    this->ui->NUM_BOXES_DoubleSpinBox->setMinimum(0.);
    this->ui->NUM_BOXES_DoubleSpinBox->setValue(this->model_2be_adjusted->num_pieces_2_num_boxes(CURR_NUM_PIECES));

    this->ui->MAX_NUM_PIECES_LineEdit->setText(QString::number(model_2be_adjusted->NUM_LEFT_PIECES));
    this->ui->NUM_PIECES_SpinBox->setMinimum(0.);
    this->ui->NUM_PIECES_SpinBox->setMaximum(CURR_NUM_PIECES);
    this->ui->NUM_PIECES_SpinBox->setValue(CURR_NUM_PIECES);

    this->ui->NUM_PIECES_PER_BOX_LineEdit->setText(QString::number(model_2be_adjusted->NUM_PIECES_PER_BOX));

    this->ui->MODEL_CODE_LineEdit->setText(model_2be_adjusted->MODEL_CODE);

    this->ui->DESCRIPTION_CN_TextEdit->setText(model_2be_adjusted->DESCRIPTION_CN);
    this->ui->DESCRIPTION_SPAN_TextEdit->setText(model_2be_adjusted->DESCRIPTION_SPAN);

    this->ui->PREZE_LineEdit->setText(QString::number(model_2be_adjusted->PRIZE, 'f', 2));

    this->ui->TOTAL_LineEdit->setText(QString::number(model_2be_adjusted->PRIZE * entry_2be_adjusted->NUM_PIECES, 'f', 2));
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
        entry_2be_adjusted->NUM_PIECES = this->ui->NUM_ITEMS_SB->value();
        entry_2be_adjusted->CAJA = ((double)this->ui->NUM_ITEMS_SB->value()) / ((double)this->entry_2be_adjusted->CANT_POR_CAJA);
        entry_2be_adjusted->TOTAL = this->ui->TOTAL_LE->text().toDouble();
    }

    // we need to update the entry of corresponding model
    this->close();
}



void Adjust_List_Item_Win::on_NUM_BOXES_SB_valueChanged(double num_boxes)
{
    unsigned long num_items = num_boxes * this->model_2be_adjusted->NUM_ITEMS_PER_BOX;
    // set number of pieces
    this->ui->NUM_ITEMS_SB->setValue(num_items);

    const double total = num_items * this->model_2be_adjusted->PRIZE;
    this->ui->TOTAL_LE->setText(QString::number(total, 'f', 2));
}

