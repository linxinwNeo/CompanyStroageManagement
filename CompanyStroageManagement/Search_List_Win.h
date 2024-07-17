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

    const unsigned int searched_lists_table_CreationDateTime_idx = 1;

    void view_selected_list();

    void clear_selected_list_info();
    
    void clear_tables();

    void clear_search_criterion();

    void set_GUI_Language();
    void setWindow();

private:
    Ui::Search_List_Win *ui;

    MainWindow* parentWin = nullptr;

    QTableWidget* search_result_Table = nullptr;
    QTableWidget* selected_list_entries_Table = nullptr;

    ListPtr selected_list = nullptr;

    const float widthRatio = 0.6;
    const float heightRatio = 0.8;

private slots:
    void closeEvent (QCloseEvent *event);

    void on_search_result_Table_cellClicked(int row, int column);
    void on_pushButton_delete_list_clicked();
    void on_pushButton_put_back_list_clicked();
    void on_pushButton_createPDF_clicked();
    void on_pushButton_SearchListsByListIDPrefix_clicked();
    void on_pushButton_SearchListsByClientIDPrefix_clicked();
    void on_pushButton_SearchListsByClientNamePrefix_clicked();
};

#endif // SEARCH_LIST_WIN_H
