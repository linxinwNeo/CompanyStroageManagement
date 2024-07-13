#include "AuthenticationWin.h"
#include "ui_AuthenticationWin.h"

#include <QMessageBox>
#include <QScreen>
#include "GlobalVars.h"

AuthenticationWin::AuthenticationWin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthentucationWin)
{
    ui->setupUi(this);

    password_correct = false;

    this->set_GUI_Language();

    this->setWindow();
}

AuthenticationWin::~AuthenticationWin()
{
    delete ui;
}


void AuthenticationWin::setWindow()
{
    QScreen *screen = QApplication::screens().at(0);
    QRect screenSize = screen->availableGeometry();

    int width = static_cast<int>(screenSize.width() * widthRatio);
    int height = static_cast<int>(screenSize.height() * heightRatio);

    this->resize(width, height);

    this->move(screenSize.width() / 2 - width / 2, screenSize.height() / 2 - height / 2);
}


// set language according to the current language
void AuthenticationWin::set_GUI_Language()
{
    this->ui->lineEdit_password->setPlaceholderText(lan("请在此输入密码","Por favor, introduzca su contraseña aquí"));
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

