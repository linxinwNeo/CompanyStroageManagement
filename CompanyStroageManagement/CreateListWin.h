#ifndef CREATELISTWIN_H
#define CREATELISTWIN_H

#include <QTableWidget>
#include <QMainWindow>
#include <QVector2D>

#include "Adjust_List_Item_Win.h"
#include "DataStructures/List.h"
#include "DataStructures/Model.h"


class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
 class CreateListWin;
}
QT_END_NAMESPACE

class CreateListWin : public QMainWindow
{
    Q_OBJECT

public:
    ListPtr list;

    const unsigned int NUM_SEARCHED_MODELS_TABLE_COLS = 11;
    const unsigned int NUM_ADDED_MODELS_TABLE_COLS = 9;

    MainWindow* parentPtr = nullptr;

    CreateListWin(QWidget *parent = nullptr);
    ~CreateListWin();

    void update_added_models_table();

    void remove_entry(const unsigned int idx);

    void setLanguage();

    void setWindow();

private slots:
    void closeEvent (QCloseEvent *event);

    void on_generatePDF_btn_clicked();
    void on_previewList_btn_clicked();

    void on_model_code_for_search_LE_textChanged(const QString &arg1);

    void on_searched_models_table_cellClicked(int row, int column);

    void on_add_selected_model_btn_clicked();

    void on_added_models_table_cellClicked(int row, int column);

    void on_remove_selected_model_btn_clicked();

    void on_reset_added_models_table_btn_clicked();

    void on_added_models_table_cellDoubleClicked(int row, int column);

private:
    void init();

    Ui::CreateListWin *ui;
    QTableWidget* searched_models_table;
    QTableWidget* added_models_table;
    ModelPtr selected_model_in_search_table;

    QSharedPointer<Adjust_List_Item_Win> adjust_list_item_win;

    void clear_searched_models_table();
    void clear_added_models_table();

    const float widthRatio = 0.6;
    const float heightRatio = 0.8;

public:
    // indices for searched_models_table
    const unsigned int searched_models_table_MODELCODE_idx = 0;
    const unsigned int searched_models_table_DESCRIPTION_CN_idx = 1;
    const unsigned int searched_models_table_DESCRIPTION_SPAN_idx = 2;
    const unsigned int searched_models_table_NUM_INIT_BOXES_idx = 3;
    const unsigned int searched_models_table_NUM_ITEMS_PER_BOX_idx = 4;
    const unsigned int searched_models_table_PRIZE_idx = 5;
    const unsigned int searched_models_table_NUM_SOLD_BOXES_idx = 6;
    const unsigned int searched_models_table_NUM_SOLD_ITEMS_idx = 7;
    const unsigned int searched_models_table_NUM_LEFT_BOXES_idx = 8;
    const unsigned int searched_models_table_NUM_LEFT_ITEMS_idx = 9;
    const unsigned int searched_models_table_ContainerID_idx = 10;

    // indices for added_models_table
    const unsigned int added_models_table_NUM_BOXES_idx = 0;
    const unsigned int added_models_table_NUM_ITEMS_idx = 1;
    const unsigned int added_models_table_NUM_ITEMS_PER_BOX_idx = 2;
    const unsigned int added_models_table_MODELCODE_idx = 3;
    const unsigned int added_models_table_DESCRIPTION_SPAN_idx = 4;
    const unsigned int added_models_table_DESCRIPTION_CN_idx = 5;
    const unsigned int added_models_table_PRIZE_idx = 6;
    const unsigned int added_models_table_TOTAL_idx = 7; // total prize
    const unsigned int added_models_table_ContainerID_idx = 8;


};

#endif // CREATELISTWIN_H
