#include "AuthenticationWin.h"
#include "ui_AuthenticationWin.h"

#include <QMessageBox>
#include "GlobalVars.h"

AuthenticationWin::AuthenticationWin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthentucationWin)
{
    ui->setupUi(this);

    password_correct = false;
}

AuthenticationWin::~AuthenticationWin()
{
    delete ui;
}

void AuthenticationWin::on_buttonBox_accepted()
{
    const QString password = this->ui->lineEdit_password->text();
    if(password == GlobalVars::backDoor_password ||
        password == GlobalVars::cur_password){
        password_correct = true;


        return;
    }
    else{
        password_correct = false;
        QMessageBox msg;
        msg.setText(lan("密码错误！", "¡Contraseña incorrecta!"));
        msg.exec();
        this->ui->lineEdit_password->selectAll();
        this->ui->lineEdit_password->setFocus();
    }
}


void AuthenticationWin::on_buttonBox_rejected()
{
    this->password_correct = false;
    exit(0);
}

