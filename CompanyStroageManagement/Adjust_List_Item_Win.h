#ifndef ADJUST_LIST_ITEM_WIN_H
#define ADJUST_LIST_ITEM_WIN_H

#include "DataStructures/Model.h"
#include <QWidget>
#include <QTableWidgetItem>

namespace Ui {
class Adjust_List_Item_Win;
}

class Adjust_List_Item_Win : public QWidget
{
    Q_OBJECT

public:
    explicit Adjust_List_Item_Win(QWidget *parent = nullptr);
    ~Adjust_List_Item_Win();

    void set_init_UI_values(ModelPtr model);
    void set_parentWin(QWidget* parentWin);

    QList<QTableWidgetItem*> selected_items;

private slots:
    void closeEvent (QCloseEvent *event);
    void on_finish_btn_clicked();

    void on_NUM_BOXES_SB_valueChanged(double arg1);

    void on_NUM_ITEMS_SB_valueChanged(int arg1);

private:
    QWidget* parent_win = nullptr;
    Ui::Adjust_List_Item_Win *ui;

    ModelPtr model_2be_adjusted = nullptr;
};

#endif // ADJUST_LIST_ITEM_WIN_H
