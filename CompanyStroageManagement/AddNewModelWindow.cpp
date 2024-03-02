#include "AddNewModelWindow.h"
#include <QCloseEvent>
#include "CN_Strings.h"
#include "FileLoader/WriteFile.h"
#include "SpanStrings.h"
#include "mainwindow.h"
#include "ui_AddNewModelWindow.h"
#include "GlobalVars.h"


AddNewModelWindow::AddNewModelWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewModelWindow)
{
    ui->setupUi(this);

    this->setLanguage();

    this->setWindow();
}

void AddNewModelWindow::setWindow()
{
    QScreen *screen = QApplication::screens().at(0);
    QRect screenSize = screen->availableGeometry();

    int width = static_cast<int>(screenSize.width() * widthRatio);
    int height = static_cast<int>(screenSize.height() * heightRatio);

    this->resize(width, height);

    this->move(screenSize.width() / 2 - width / 2, screenSize.height() / 2 - height / 2);
}


// set language according to the current language
void AddNewModelWindow::setLanguage()
{
    const QString none = lan("暂无", "ninguno");

    this->ui->newModel_info_GB->setTitle(lan("新货物的信息", "información de los nuevos productos"));

    this->ui->MODELCODE_label->setText(lan("货号", "MODELO"));
    this->ui->MODELCODE_LE->setPlaceholderText(none);

    this->ui->DESCRIPTION_CN_label->setText(lan("品名（中文）", "DESCRIPTION(Chino)"));
    this->ui->DESCRIPTION_CN_LE->setPlaceholderText(none);

    this->ui->DESCRIPTION_SPAN_label->setText(lan("品名（西语）", "DESCRIPTION(Español)"));
    this->ui->DESCRIPTION_SPAN_LE->setPlaceholderText(none);

    this->ui->PRIZE_label->setText(lan("单价", "precio del artículo"));
    this->ui->NUM_INIT_BOXES_label->setText(lan("初始箱数", "Número inicial de cajas"));
    this->ui->NUM_SOLD_BOXES_label->setText(lan("已售箱数", "Número de cajas vendidas"));
    this->ui->NUM_ITEMS_PER_BOX_label->setText(lan("每箱个数", "Piezas por caja"));

    this->ui->container_ID_label->setText(lan("集装箱号", "número de contenedor"));

    this->ui->container_ID_LE->setPlaceholderText(lan("在此输入集装箱号，如果没有的话空着",
                                                      "Ingrese el número de contenedor aquí, si no tiene uno, deje este espacio en blanco"));

    this->ui->add_new_model_btn->setText(lan("添加", "añadir"));
}


AddNewModelWindow::~AddNewModelWindow()
{
    delete ui;

    if(this->parentPtr != nullptr) {
        this->parentPtr->update_GUI();
        this->parentPtr->show();
    }
}


// clear the contents of user inputs
void AddNewModelWindow::clear_content()
{
    this->ui->MODELCODE_LE->clear();
    this->ui->DESCRIPTION_CN_LE->clear();
    this->ui->DESCRIPTION_SPAN_LE->clear();
    this->ui->PRIZE_SB->setValue(0.);
    this->ui->NUM_INIT_BOXES_SB->setValue(0.);
    this->ui->NUM_SOLD_BOXES_SB->setValue(0.);
    this->ui->NUM_ITEMS_PER_BOX_SB->setValue(0.);
    this->ui->DESCRIPTION_SPAN_LE->clear();
    this->ui->container_ID_LE->clear();
}


QSharedPointer<QMessageBox> AddNewModelWindow::create_MessageBox(const QString &msg)
{
    QSharedPointer<QMessageBox> Msgbox(new QMessageBox(this));
    Msgbox->setText(msg);
    return Msgbox;
}


void AddNewModelWindow::closeEvent(QCloseEvent *event)
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


/* 先对输入的数据进行查错，没有问题的话就创建一个新的货物
 * 可能还有一个新的集装箱 */
void AddNewModelWindow::on_add_new_model_btn_clicked()
{

    // 检查货号是否是空
    const QString MODELCODE = this->ui->MODELCODE_LE->text().trimmed();

    if(MODELCODE.isEmpty()) {
        QString msg = lan(ADD_NEW_MODEL_FAIL_MSG_CN, ADD_NEW_MODEL_FAIL_MSG_SPAN) + lan(MODELCODE_EMPTY_ERROR_CN, MODELCODE_EMPTY_ERROR_SPAN);

        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    const QString CONTAINER_ID = this->ui->container_ID_LE->text().trimmed();


    // 检查货号和对应的集装箱组合是否已经存在
    const ModelPtr model = inventory.get_Model(MODELCODE, CONTAINER_ID);
    if(!model.isNull()){
        QString msg = lan(ADD_NEW_MODEL_FAIL_MSG_CN, ADD_NEW_MODEL_FAIL_MSG_SPAN) + lan(duplicate_Model_ERROR_MSG_CN, duplicate_Model_ERROR_MSG_SPAN);

        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    // 提取其他参数
    const QString DESCRIPTION_CN = this->ui->DESCRIPTION_CN_LE->text().trimmed();
    const QString DESCRIPTION_SPAN = this->ui->DESCRIPTION_SPAN_LE->text().trimmed();
    const double PRIZE = this->ui->PRIZE_SB->value();
    const double NUM_INIT_BOXES = this->ui->NUM_INIT_BOXES_SB->value();
    const double NUM_SOLD_BOXES = this->ui->NUM_SOLD_BOXES_SB->value();
    const unsigned long int NUM_ITEMS_PER_BOX = this->ui->NUM_ITEMS_PER_BOX_SB->value();
    const double NUM_LEFT_BOXES = NUM_INIT_BOXES - NUM_SOLD_BOXES;
    const unsigned long int NUM_LEFT_ITEMS = NUM_LEFT_BOXES * NUM_ITEMS_PER_BOX;

    // 初始箱数不能少于已售箱数
    if(NUM_SOLD_BOXES > NUM_INIT_BOXES){
        QString msg = lan(ADD_NEW_MODEL_FAIL_MSG_CN, ADD_NEW_MODEL_FAIL_MSG_SPAN) + lan(SOLD_MORETHAN_INIT_BOXES_ERROR_MSG_CN, SOLD_MORETHAN_INIT_BOXES_ERROR_MSG_SPAN);
        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    // 创建新的货物
    ModelPtr new_model (new Model(MODELCODE, DESCRIPTION_SPAN, DESCRIPTION_CN,
                                 PRIZE, NUM_INIT_BOXES, NUM_SOLD_BOXES, NUM_LEFT_BOXES,
                                 NUM_LEFT_ITEMS, NUM_ITEMS_PER_BOX));

    // 还缺少集装箱没处理

    // 检查集装箱是否存在， 如果存在就直接引用，不存在就创建新的
    ContainerPtr container(nullptr);

    if(CONTAINER_ID.isEmpty()){ // 这个货没有集装箱信息
        container = nullptr;
    }
    else{ // 这个货有集装箱信息
        container = inventory.get_container(CONTAINER_ID);
        if(container.isNull()){ // 这个集装箱是新的，我们要创建一个
            container = ContainerPtr (new Container(CONTAINER_ID));
            container->add_model(new_model);
            inventory.add_Container(container);
        }
        else{ // 这个集装箱不是新的，我们要创建一个
            container->add_model(new_model);
        }
    }

    new_model->container = container;
    inventory.add_Model(new_model);


    QString msg = lan(NEW_MODEL_ADD_SUCCESS_MSG_CN, NEW_MODEL_ADD_SUCCESS_MSG_SPAN);

    QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
    msgBox->exec();

    // clear the contents when the model is successfully added
    this->clear_content();

    // save the inventory
    if( !WriteFile::SaveInventoryAuto(false) ) {
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(lan(SAVE_ERROR_MSG_CN, SAVE_ERROR_MSG_SPAN));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }
}

