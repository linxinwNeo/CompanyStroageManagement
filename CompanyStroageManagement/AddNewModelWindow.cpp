#include "AddNewModelWindow.h"
#include <QCloseEvent>
#include "LanguageStrings.h"
#include "FileLoader/WriteFile.h"
#include "mainwindow.h"
#include "ui_AddNewModelWindow.h"
#include "GlobalVars.h"


AddNewModelWindow::AddNewModelWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewModelWindow)
{
    ui->setupUi(this);
    
    this->set_GUI_Language();

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
void AddNewModelWindow::set_GUI_Language()
{
    const QString none = lan("暂无", "ninguno");

    this->ui->newModel_info_GB->setTitle(lan("新货物的信息",
                                             "información de los nuevos productos"));

    this->ui->label_MODELCODE->setText(lan("货号", "MODELO"));
    this->ui->lineEdit_MODELCODE->setPlaceholderText(none);

    this->ui->label_DESCRIPTION_CN->setText(lan("品名（中文）",
                                                "Nombre del producto (en chino)"));
    this->ui->lineEdit_DESCRIPTION_CN->setPlaceholderText(none);

    this->ui->label_DESCRIPTION_SPAN->setText(lan("品名（西语）",
                                                  "Nombre del producto (en español)"));
    this->ui->lineEdit_DESCRIPTION_SPAN->setPlaceholderText(none);

    this->ui->label_PRIZE->setText(lan("单价", "PRECIO"));

    this->ui->label_NUM_INIT_BOXES->setText(lan("进货箱数", "ENTRADAS"));
    this->ui->label_NUM_SOLD_BOXES->setText(lan("已售箱数", "SALIDAS"));

    this->ui->label_NUM_INIT_PIECES->setText(lan("进货个数", "Núm. piezas adq."));
    this->ui->label_NUM_SOLD_PIECES->setText(lan("已售个数", "Núm. piezas vend."));
    this->ui->label_NUM_PIECES_PER_BOX->setText(lan("每箱个数", "EMPAQUE"));

    this->ui->label_container_ID->setText(lan("集装箱号", "Número de contenedor"));

    this->ui->lineEdit_container_ID->setPlaceholderText(lan("在此输入集装箱号，如果没有的话空着",
                                                      "Ingrese el número de contenedor aquí, si no tiene uno, deje este espacio en blanco"));

    this->ui->button_add_new_model->setText(lan("添加", "Añadir"));
}


AddNewModelWindow::~AddNewModelWindow()
{
    delete ui;
}


// clear the contents of ui (values...)
void AddNewModelWindow::clear_content()
{
    this->ui->lineEdit_MODELCODE->clear();
    this->ui->lineEdit_DESCRIPTION_CN->clear();
    this->ui->lineEdit_DESCRIPTION_SPAN->clear();
    this->ui->lineEdit_container_ID->clear();


    this->ui->lineEdit_NUM_INIT_PIECES->setText("1.");
    this->ui->lineEdit_NUM_SOLD_PIECES->setText("0");

    this->ui->spinBox_NUM_PIECES_PER_BOX->setValue(1);
    this->ui->doubleSpinBox_NUM_INIT_BOXES->setValue(1.);
    this->ui->doubleSpinBox_NUM_SOLD_BOXES->setValue(0);
    this->ui->doubleSpinBox_PRIZE->setValue(1.);
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
void AddNewModelWindow::on_button_add_new_model_clicked()
{

    // 检查货号是否是空
    const QString MODELCODE = this->ui->lineEdit_MODELCODE->text().trimmed();

    if(MODELCODE.isEmpty()) {
        QString msg = lan(ADD_NEW_MODEL_FAIL_MSG_CN, ADD_NEW_MODEL_FAIL_MSG_SPAN) + lan(MODELCODE_EMPTY_ERROR_CN, MODELCODE_EMPTY_ERROR_SPAN);

        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    // 进货箱数必须大于0！
    if(this->ui->doubleSpinBox_NUM_INIT_BOXES->value() <= 0.){
        QString msg = lan("进货箱数必须大于0！",
                          "¡El número de cajas entrantes debe ser mayor que 0!");

        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    // 进货箱数必须大于等于已出售箱数！
    if(this->ui->doubleSpinBox_NUM_INIT_BOXES->value() < this->ui->doubleSpinBox_NUM_SOLD_BOXES->value()){
        QString msg = lan("已出售箱数不能比进货箱数更多，请检查数量！",
                          "El número de cajas vendidas no puede ser mayor que el número de cajas entrantes, ¡así que verifique la cantidad!");

        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    // 每箱个数不必须大于0！
    if(this->ui->spinBox_NUM_PIECES_PER_BOX->value() <= 0){
        QString msg = lan("每箱个数必须大于0，请检查数量！",
                          "El número de piezas por caja debe ser superior a 0, ¡compruebe la cantidad!");

        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    const QString CONTAINER_ID = this->ui->lineEdit_container_ID->text().trimmed();


    // 检查货号和对应的集装箱组合是否已经存在
    const ModelPtr model = inventory.get_Model(MODELCODE, CONTAINER_ID);
    if(!model.isNull()){
        QString msg = lan(ADD_NEW_MODEL_FAIL_MSG_CN, ADD_NEW_MODEL_FAIL_MSG_SPAN) + lan(duplicate_Model_ERROR_MSG_CN, duplicate_Model_ERROR_MSG_SPAN);

        QSharedPointer<QMessageBox> msgBox = this->create_MessageBox(msg);
        msgBox->exec();
        return;
    }

    // 提取其他参数
    extern QLocale locale;

    const QString DESCRIPTION_CN = this->ui->lineEdit_DESCRIPTION_CN->text().trimmed();
    const QString DESCRIPTION_SPAN = this->ui->lineEdit_DESCRIPTION_SPAN->text().trimmed();
    const double PRIZE = this->ui->doubleSpinBox_PRIZE->value();
    const unsigned long NUM_INIT_PIECES = locale.toULong(this->ui->lineEdit_NUM_INIT_PIECES->text());
    const unsigned long NUM_SOLD_PIECES = locale.toULong(this->ui->lineEdit_NUM_SOLD_PIECES->text());
    const unsigned long NUM_PIECES_PER_BOX = this->ui->spinBox_NUM_PIECES_PER_BOX->value();

    // 创建新的货物
    ModelPtr new_model (new Model( MODELCODE,
                                 DESCRIPTION_SPAN,
                                 DESCRIPTION_CN,
                                 PRIZE,
                                 NUM_INIT_PIECES,
                                 NUM_SOLD_PIECES,
                                 NUM_PIECES_PER_BOX));

    // 还缺少集装箱没处理

    // 检查集装箱是否存在， 如果存在就直接引用，不存在就创建新的
    ContainerPtr container(nullptr);

    if(CONTAINER_ID.isEmpty()){ // 这个货没有集装箱信息
        container = nullptr;
    }
    else{ // 这个货有集装箱信息
        container = inventory.get_container(CONTAINER_ID);
        if(container.isNull()){ // 这个集装箱是不存在的，我们要创建一个
            container = ContainerPtr (new Container(CONTAINER_ID));
            container->add_model(new_model);
            inventory.add_Container(container);
        }
        else{ // 这个集装箱是新的，我们直接加入这个货
            container->add_model(new_model);
        }
    }

    new_model->m_Container = container;
    inventory.add_new_Model(new_model);


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

    // update the mainwindow
    this->parentPtr->update_GUI();
    // save the inventory and lists
    WriteFile::SaveInventoryAuto(false);
    WriteFile::Lists2txt(false);
}


// 当这个值改变后，我们需要更新 NUM_INIT_PIECES 的数值
void AddNewModelWindow::on_doubleSpinBox_NUM_INIT_BOXES_valueChanged(double val)
{
    double num_pieces_per_box = this->ui->spinBox_NUM_PIECES_PER_BOX->value();
    int num_init_pieces = floor(val * num_pieces_per_box);

    extern QLocale locale;
    this->ui->lineEdit_NUM_INIT_PIECES->setText(locale.toString(num_init_pieces));
}


// 当这个值改变后，我们需要更新 NUM_SOLD_PIECES 的数值
void AddNewModelWindow::on_doubleSpinBox_NUM_SOLD_BOXES_valueChanged(double val)
{
    double num_pieces_per_box = this->ui->spinBox_NUM_PIECES_PER_BOX->value();
    int num_sold_pieces = floor(val * num_pieces_per_box);

    extern QLocale locale;
    this->ui->lineEdit_NUM_SOLD_PIECES->setText(locale.toString(num_sold_pieces));
}


// 当这个值改变后，我们需要更新 NUM_INIT_PIECES 和 NUM_SOLD_PIECES 的数值
void AddNewModelWindow::on_spinBox_NUM_PIECES_PER_BOX_valueChanged(int val)
{
    const double num_init_boxes = this->ui->doubleSpinBox_NUM_INIT_BOXES->value();
    const double num_sold_boxes = this->ui->doubleSpinBox_NUM_SOLD_BOXES->value();

    int num_init_pieces = floor(((double)val) * num_init_boxes);
    int num_sold_pieces = floor(((double)val) * num_sold_boxes);

    extern QLocale locale;

    this->ui->lineEdit_NUM_INIT_PIECES->setText(locale.toString(num_init_pieces));
    this->ui->lineEdit_NUM_SOLD_PIECES->setText(locale.toString(num_sold_pieces));
}

