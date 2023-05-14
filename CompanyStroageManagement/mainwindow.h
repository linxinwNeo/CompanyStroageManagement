#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client_info.h"
#include "entrylist.h"
#include <qtablewidget.h>
#include <QMainWindow>
#include <QVector2D>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern EntryList EL;
extern Client_info client_info;
extern const QString APP_NAME;

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

private slots:
    void on_add_entry_btn_released();

    void on_reset_entry_btn_released();

    void on_reset_info_btn_released();

    void on_reset_table_btn_released();

    void on_delete_row_btn_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_generatePDF_btn_clicked();

    void closeEvent (QCloseEvent *event);


private:
    Ui::MainWindow *ui;
    QTableWidget* table;
};
#endif // MAINWINDOW_H
