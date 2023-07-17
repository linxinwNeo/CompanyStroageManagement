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

private slots:
    void closeEvent (QCloseEvent *event);
    void on_search_MODELCODE_LE_textChanged(QString s);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
