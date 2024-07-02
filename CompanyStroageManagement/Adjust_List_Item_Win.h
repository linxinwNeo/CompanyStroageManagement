#ifndef ADJUST_LIST_ITEM_WIN_H
#define ADJUST_LIST_ITEM_WIN_H

#include "DataStructures/Model.h"
#include "DataStructures/Entry.h"
#include <QWidget>
#include <QTableWidgetItem>

class CreateListWin;
namespace Ui {
class Adjust_List_Item_Win;
}

class Adjust_List_Item_Win : public QWidget
{
    Q_OBJECT

public:
    explicit Adjust_List_Item_Win(QWidget *parent = nullptr);
    ~Adjust_List_Item_Win();
    
    void set_model_and_entry(ModelPtr model, EntryPtr entry, const unsigned int entry_idx);
    
    void set_GUI_values();

    CreateListWin* parent_win = nullptr;
    void setLanguage();
    void setWindow();

private slots:
    void closeEvent (QCloseEvent *event);
    void on_finish_btn_clicked();
    
    void on_NUM_BOXES_DoubleSpinBox_valueChanged(double arg1);

    void on_NUM_PIECES_SpinBox_valueChanged(int arg1);

private:
    Ui::Adjust_List_Item_Win *ui;

    ModelPtr model_2be_adjusted = nullptr;
    EntryPtr entry_2be_adjusted = nullptr;
    unsigned int entry_idx = -1;

    const float widthRatio = 0.3;
    const float heightRatio = 0.8;
};

#endif // ADJUST_LIST_ITEM_WIN_H
