#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataStructures/Model.h"
#include "DataStructures/client_info.h"
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
    void show_selected_model();
    void clear_selected_model();

private slots:
    void closeEvent (QCloseEvent *event);
    void on_search_MODELCODE_LE_textChanged(QString new_str);

    void on_update_selected_model_btn_clicked();

    void on_search_model_result_Table_cellClicked(int row, int column);

private:
    ModelPtr selected_model;
    unsigned int num_search_model_result_table_columns = 11;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
