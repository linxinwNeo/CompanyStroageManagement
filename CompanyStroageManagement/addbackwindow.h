#ifndef ADDBACKWINDOW_H
#define ADDBACKWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class AddbackWindow;
}

class AddbackWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddbackWindow(QWidget *parent = nullptr);
    ~AddbackWindow();

    void reset_entry_form();

private slots:
    void closeEvent (QCloseEvent *event);

    void on_add_entry_btn_clicked();

    void on_reset_entry_btn_clicked();

    void on_addback_btn_clicked();

    void on_delete_row_btn_clicked();

    void on_reset_table_btn_clicked();

    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::AddbackWindow *ui;
    QTableWidget* table;
};

#endif // ADDBACKWINDOW_H
