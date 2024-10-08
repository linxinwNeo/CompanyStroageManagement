#ifndef AUTHENTICATIONWIN_H
#define AUTHENTICATIONWIN_H

#include <QDialog>

namespace Ui {
class AuthentucationWin;
}

class AuthenticationWin : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenticationWin(QWidget *parent = nullptr);
    ~AuthenticationWin();

    void set_GUI_Language();
    void setWindow();

    bool password_correct;
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AuthentucationWin *ui;

    const float widthRatio = 0.2;
    const float heightRatio = 0.2;
};

#endif // AUTHENTICATIONWIN_H
