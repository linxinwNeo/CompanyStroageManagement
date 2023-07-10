#ifndef CREATELIST_H
#define CREATELIST_H

#include "DataStructures/client_info.h"
#include <qtablewidget.h>
#include <QMainWindow>
#include <QVector2D>

QT_BEGIN_NAMESPACE
namespace Ui {
 class CreateList;
}
QT_END_NAMESPACE

class CreateList : public QMainWindow
{
    Q_OBJECT

public:
    CreateList(QWidget *parent = nullptr);
    ~CreateList();

    double get_discount_value() const;
    double get_total() const;

    QString currDate() const;
    QString currTime() const;
    void create_pdf(QString filename);


private slots:
    void closeEvent (QCloseEvent *event);

    void on_generatePDF_btn_clicked();

private:
    Ui::CreateList *ui;
    QTableWidget* table;
};

#endif // CREATELIST_H
