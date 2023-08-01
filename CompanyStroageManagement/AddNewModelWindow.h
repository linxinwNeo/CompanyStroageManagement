#ifndef ADDNEWMODELWINDOW_H
#define ADDNEWMODELWINDOW_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class AddNewModelWindow;
}

class AddNewModelWindow : public QWidget
{
    Q_OBJECT

public:
    QWidget* parentPtr = nullptr;

    QSharedPointer<QMessageBox> create_MessageBox(const QString& msg);
    explicit AddNewModelWindow(QWidget *parent = nullptr);
    ~AddNewModelWindow();

    void clear_content();
private:
    Ui::AddNewModelWindow *ui;

private slots:
    void closeEvent (QCloseEvent *event);
    void on_add_new_model_btn_clicked();
};

#endif // ADDNEWMODELWINDOW_H
