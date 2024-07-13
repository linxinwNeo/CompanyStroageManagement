#include "ChangePasswordWin.h"
#include "ui_ChangePasswordWin.h"

#include <QCloseEvent>
#include <QMessageBox>

#include "GlobalVars.h"
#include "FileLoader/WriteFile.h"
#include "Others/write_error_file.h"
#include "mainwindow.h"

ChangePasswordWin::ChangePasswordWin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChangePasswordWin)
{
    ui->setupUi(this);
}

ChangePasswordWin::~ChangePasswordWin()
{
    delete ui;
}


void ChangePasswordWin::closeEvent(QCloseEvent *event)
{
    QMessageBox msg(this);
    msg.setText(lan("你确定要返回主界面吗？", "¿Seguro que quieres volver a la pantalla principal?"));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    int resBtn = msg.exec();
    if (resBtn == QMessageBox::No) {
        event->ignore();
    }
    else {
        if(this->parentPtr != nullptr) emit this->parentPtr->set_as_front_window();
        event->accept();
    }
}



// 检测密码是否合理
void ChangePasswordWin::on_pushButton_finish_clicked()
{
    const QString first_enter = this->ui->lineEdit_new_password->text();
    const QString sec_enter = this->ui->lineEdit_new_password_again->text();

    if(first_enter.isEmpty() || first_enter.length() < 6){
        // empty password is not allowed
        QMessageBox msg;
        msg.setText(lan("密码长度必须大于6位！", "¡La contraseña debe tener más de 6 caracteres!"));
        msg.exec();
    }

    else if(first_enter != sec_enter){
        QMessageBox msg;
        msg.setText(lan("两次输入的密码不匹配！", "¡La contraseña ingresada dos veces no coincide!"));
        msg.exec();
    }

    else if(first_enter == sec_enter){
        // 保存新密码到global vars里
        GlobalVars::cur_password = first_enter;

        // 保存新密码到setting里
        if(WriteFile::save_settings_file()){
            QMessageBox msg;
            msg.setText(lan("新密码设置成功！", "¡La nueva contraseña se ha establecido con éxito!"));
            msg.exec();
            this->parentPtr->show();
            this->close();
        }
        else{
            QMessageBox msg;
            msg.setText(lan("新密码保存失败!重启可能可以解决该问题。",
                            "¡No se pudo guardar la nueva contraseña! Un reinicio puede resolver el problema."));
            msg.exec();
            write_error_file("新密码保存失败，原因是无法保存至settings.txt.");
        }
    }

    return;
}

