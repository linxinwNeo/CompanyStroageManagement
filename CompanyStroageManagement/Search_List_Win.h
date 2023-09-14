#ifndef SEARCH_LIST_WIN_H
#define SEARCH_LIST_WIN_H

#include <QWidget>
#include <QTableWidget>
#include "DataStructures/List.h"

namespace Ui {
class Search_List_Win;
}

class MainWindow;

class Search_List_Win : public QWidget
{
    Q_OBJECT

public:
    explicit Search_List_Win(QWidget *parent = nullptr);
    ~Search_List_Win();

    void set_parentWin(MainWindow* win);

    unsigned int searched_lists_table_list_id_idx = 0;

    void view_selected_list();

    void reset_list_info();

private:
    Ui::Search_List_Win *ui;

    MainWindow* parentWin = nullptr;

    QTableWidget* searched_lists_table = nullptr;
    QTableWidget* list_models_table = nullptr;

    ListPtr selected_list = nullptr;

private slots:
    void closeEvent (QCloseEvent *event);

    void on_list_id_2be_searched_LE_textChanged(const QString &arg1);
    void on_searched_lists_table_cellClicked(int row, int column);
    void on_delete_list_btn_clicked();
    void on_put_back_list_btn_clicked();
};

#endif // SEARCH_LIST_WIN_H
