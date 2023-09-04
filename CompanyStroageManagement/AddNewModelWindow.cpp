#include "AddNewModelWindow.h"
#include <QCloseEvent>
#include "ui_AddNewModelWindow.h"
#include "GlobalVars.h"


AddNewModelWindow::AddNewModelWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewModelWindow)
{
    ui->setupUi(this);
}


AddNewModelWindow::~AddNewModelWindow()
{
    delete ui;

    if(this->parentPtr != nullptr) this->parentPtr->show();
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
}


QSharedPointer<QMessageBox> AddNewModelWindow::create_MessageBox(const QString &msg)
{
    QSharedPointer<QMessageBox> Msgbox(new QMessageBox(this));
    Msgbox->setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");
    Msgbox->setText(msg);
    return Msgbox;
}


void AddNewModelWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msg(this);
    msg.setText(tr("你确定要退出吗?\n"));
    msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    msg.setStyleSheet("QLabel{min-width: 200px; min-height: 50px;}");

    int resBtn = msg.exec();
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    }
    else {
        if(this->parentPtr != nullptr) this->parentPtr->show(); // show the mainwindow
        event->accept();
    }
}


/* 先对输入的数据进行差错，没有问题的话就创建一个新的货物
 * 可能还有一个新的集装箱 */
void AddNewModelWindow::on_add_new_model_btn_clicked()
{
    const QString msg = "添加失败！";

    // 检查货号是否是空
    const QString MODELCODE = this->ui->MODELCODE_LE->text().trimmed();

    if(MODELCODE.isEmpty()) {
        auto msgBox = this->create_MessageBox(msg + empty_MODELCODE_ERROR_MSG);
        msgBox->exec();
        return;
    }

    const QString CONTAINER_ID = this->ui->container_ID_LE->text().trimmed();


    // 检查货号和对应的集装箱组合是否已经存在
    const ModelPtr model = inventory.get_Model(MODELCODE, CONTAINER_ID);
    if(!model.isNull()){
        auto msgBox = this->create_MessageBox(msg + duplicate_Model_ERROR_MSG);
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
        auto msgBox = this->create_MessageBox(msg + SOLD_MORETHAN_INIT_BOXES_ERROR_MSG);
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

    if(CONTAINER_ID.isNull()){ // 这个货没有集装箱信息
        container = nullptr;
    }
    else{ // 这个货有集装箱信息
        container = inventory.get_container(CONTAINER_ID);
        if(container.isNull()){ // 这个集装箱是新的，我们要创建一个
            container = QSharedPointer<Container> (new Container(CONTAINER_ID));
            container->add_model(new_model);
            inventory.add_Container(container);
        }
        else{ // 这个集装箱不是新的，我们要创建一个
            container->add_model(new_model);
        }
    }

    new_model->container = container;
    inventory.add_Model(new_model);

    auto msgBox = this->create_MessageBox(NEW_MODEL_ADD_SUCCESS_MSG);
    msgBox->exec();

    // clear the contents when the model is successfully added
    this->clear_content();
}

