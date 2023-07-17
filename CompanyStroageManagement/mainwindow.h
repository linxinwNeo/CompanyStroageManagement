#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void closeEvent (QCloseEvent *event);
    void on_search_MODELCODE_LE_textChanged(QString new_str);

    void on_search_model_result_Table_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
