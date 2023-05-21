#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataStructures/client_info.h"
#include <qtablewidget.h>
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

    LD get_discount_value() const;
    LD get_total() const;

    QString currDate() const;
    QString currTime() const;
    void create_pdf(QString filename);
    void reset_entry();
    QString get_stroage_excel_file();

private slots:
    void on_add_entry_btn_released();

    void on_reset_entry_btn_released();

    void on_reset_info_btn_released();

    void on_reset_table_btn_released();

    void on_delete_row_btn_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_generatePDF_btn_clicked();

    void closeEvent (QCloseEvent *event);

    void on_is_preview_CB_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QTableWidget* table;
};
#endif // MAINWINDOW_H
