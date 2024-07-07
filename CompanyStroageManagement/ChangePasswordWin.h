#ifndef CHANGEPASSWORDWIN_H
#define CHANGEPASSWORDWIN_H

#include <QWidget>

namespace Ui {
class ChangePasswordWin;
}

class ChangePasswordWin : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePasswordWin(QWidget *parent = nullptr);
    ~ChangePasswordWin();

    QWidget* parentPtr;

private slots:
    void on_pushButton_finish_clicked();

private:
    Ui::ChangePasswordWin *ui;
};

#endif // CHANGEPASSWORDWIN_H
