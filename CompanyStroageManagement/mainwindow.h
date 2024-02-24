#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AddNewModelWindow.h"
#include "CreateListWin.h"
#include "DataStructures/Model.h"
#include "DataStructures/Container.h"
#include "Search_List_Win.h"
#include <QTableWidget>
#include <QMainWindow>
#include <QVector2D>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void set_Window();

    void show_selected_model();
    void clear_selected_model();
    void clear_search_model_result_table();

    void show_selected_container();
    void clear_selected_container_table();
    void clear_search_container_result_table();

    void update_GUI();

    void setLanguage();

private slots:
    void closeEvent (QCloseEvent *event);
    void on_search_MODELCODE_LE_textChanged(const QString& new_str);

    void on_update_selected_model_btn_clicked();

    void on_search_model_result_Table_cellClicked(int row, int column);

    void on_search_CONTAINER_ID_LE_textChanged(const QString& new_str);

    void on_search_container_result_Table_cellClicked(int row, int column);

    void on_start_add_model_btn_clicked();

    void on_new_list_btn_clicked();

    void on_search_past_list_btn_clicked();

    void on_delete_model_btn_clicked();

    void on_save2_new_file_btn_clicked();

    void on_read_from_new_file_btn_clicked();

    void on_actionSpanish_triggered();
    void on_actionChinese_triggered();

    void on_action_Save_Inventory_triggered();

    void on_action_Save_Lists_triggered();

    void on_save_lists_2_new_file_btn_clicked();

    void on_read_lists_from_new_file_btn_clicked();

private:
    ModelPtr selected_model;
    ContainerPtr selected_container;
    unsigned int num_search_model_result_table_columns = 11;
    unsigned int num_search_container_result_table_columns = 6;
    Ui::MainWindow *ui;
    QSharedPointer<AddNewModelWindow> AddNewModelWinPtr;
    QSharedPointer<CreateListWin> CreateListWinPtr;
    QSharedPointer<Search_List_Win> SearchListWinPtr;

    bool is_time_for_backup() const;

    const float widthRatio = 0.6;
    const float heightRatio = 0.8;

public:
    const unsigned int search_model_result_table_MODELCODE_idx = 0;
    const unsigned int search_model_result_table_ContainerID_idx = 10;

    const unsigned int search_container_result_table_ContainerID_idx = 0;
};
#endif // MAINWINDOW_H
