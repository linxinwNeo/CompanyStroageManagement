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

    MainWindow* parentPtr = nullptr;

    CreateListWin(QWidget *parent = nullptr);
    ~CreateListWin();

    void update_added_models_table();

    void remove_entry(const unsigned int idx);
    
    void set_GUI_Language();

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

    void on_reset_client_info_btn_clicked();

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
};

#endif // CREATELISTWIN_H
