#ifndef ADDNEWMODELWINDOW_H
#define ADDNEWMODELWINDOW_H

#include <QWidget>
#include <QMessageBox>

class MainWindow;

namespace Ui {
class AddNewModelWindow;
}

class AddNewModelWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow* parentPtr = nullptr;

    QSharedPointer<QMessageBox> create_MessageBox(const QString& msg);
    explicit AddNewModelWindow(QWidget *parent = nullptr);
    ~AddNewModelWindow();

    void clear_content();
    void set_GUI_Language();
    void setWindow();
private:
    Ui::AddNewModelWindow *ui;

    const float widthRatio = 0.6;
    const float heightRatio = 0.8;

private slots:
    void closeEvent (QCloseEvent *event);
    void on_add_new_model_btn_clicked();
};

#endif // ADDNEWMODELWINDOW_H
