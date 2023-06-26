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

    double get_discount_value() const;
    double get_total() const;

    QString currDate() const;
    QString currTime() const;
    void create_pdf(QString filename);
    void reset_entry_form();
    QString get_stroage_excel_file();

private slots:
    void closeEvent (QCloseEvent *event);

    void on_generatePDF_btn_clicked();
private:
    Ui::MainWindow *ui;
    QTableWidget* table;
};
#endif // MAINWINDOW_H
