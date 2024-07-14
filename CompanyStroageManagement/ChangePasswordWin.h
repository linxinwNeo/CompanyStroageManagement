#ifndef CHANGEPASSWORDWIN_H
#define CHANGEPASSWORDWIN_H

#include <QWidget>

class MainWindow;

namespace Ui {
class ChangePasswordWin;
}

class ChangePasswordWin : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePasswordWin(QWidget *parent = nullptr);
    ~ChangePasswordWin();

    void set_GUI_Language();

    MainWindow* parentPtr;

private slots:
    void closeEvent (QCloseEvent *event);
    void on_pushButton_finish_clicked();

private:
    Ui::ChangePasswordWin *ui;
};

#endif // CHANGEPASSWORDWIN_H
