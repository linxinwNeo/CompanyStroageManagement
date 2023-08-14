#ifndef CREATELISTWIN_H
#define CREATELISTWIN_H

#include "DataStructures/List.h"
#include "DataStructures/Model.h"
#include "DataStructures/client_info.h"
#include "DataStructures/entrylist.h"
#include <qtablewidget.h>
#include <QMainWindow>
#include <QVector2D>

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

    const unsigned int NUM_SEARCHED_MODELS_TABLE_COLS = 11;
    const unsigned int NUM_ADDED_MODELS_TABLE_COLS = 8;

    QWidget* parentPtr = nullptr;

    CreateListWin(QWidget *parent = nullptr);
    ~CreateListWin();

    QString currDate() const;
    QString currTime() const;
    void create_pdf(QString filename);

private slots:
    void closeEvent (QCloseEvent *event);

    void on_generatePDF_btn_clicked();

    void on_model_code_for_search_LE_textChanged(const QString &arg1);

    void on_searched_models_table_cellClicked(int row, int column);

    void on_add_selected_model_btn_clicked();

    void on_added_models_table_cellClicked(int row, int column);

    void on_remove_selected_model_btn_clicked();

    void on_reset_added_models_table_btn_clicked();

    void on_added_models_table_cellDoubleClicked(int row, int column);

private:
    void init();

    Ui::CreateListWin *ui;
    QTableWidget* searched_models_table;
    QTableWidget* added_models_table;
    ModelPtr selected_model_in_search_table;
    ModelPtr selected_model_in_added_table;

    void clear_searched_models_table();
    void clear_added_models_table();
};

#endif // CREATELISTWIN_H
