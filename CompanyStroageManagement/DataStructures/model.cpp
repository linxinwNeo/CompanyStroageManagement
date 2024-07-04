#include "Model.h"
#include "DataStructures/Container.h"
#include "GlobalVars.h"
#include <QDebug>
#include "Others/write_error_file.h"

Model::Model()
{
    this->reset();
}

Model::Model(const QString &MODEL_CODE, const QString &DESCRIPTION_SPAN, const QString &DESCRIPTION_CN, const double &PRIZE,
             const unsigned long &NUM_INIT_PIECES, const unsigned long &NUM_SOLD_PIECES,
             const unsigned long &NUM_PIECES_PER_BOX)
{
    m_MODEL_CODE = MODEL_CODE;
    m_DESCRIPTION_SPAN = DESCRIPTION_SPAN;
    m_DESCRIPTION_CN = DESCRIPTION_CN;
    m_PRICE = PRIZE;

    m_NUM_INIT_PIECES = NUM_INIT_PIECES;
    m_NUM_SOLD_PIECES = NUM_SOLD_PIECES;

    m_NUM_PIECES_PER_BOX = NUM_PIECES_PER_BOX;

    m_last_time_modified = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());
    m_time_created = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());
    m_Container = nullptr;
}


Model::~Model()
{
    this->reset();
}


void Model::reset()
{
    m_MODEL_CODE = "";
    m_DESCRIPTION_CN = "";
    m_DESCRIPTION_SPAN = "";
    m_PRICE = 0.;
    m_NUM_INIT_PIECES = 0;
    m_NUM_SOLD_PIECES = 0;
    m_NUM_PIECES_PER_BOX = 0;

    this->m_Container = nullptr;
    this->m_last_time_modified = nullptr;
    this->m_time_created = nullptr;
}


unsigned long Model::NUM_LEFT_PIECES() const
{
    return this->m_NUM_INIT_PIECES - this->m_NUM_SOLD_PIECES;
}


double Model::NUM_INIT_BOXES() const
{
    const double num_init_pieces = this->m_NUM_INIT_PIECES;
    const double num_pieces_per_box = this->m_NUM_PIECES_PER_BOX;
    return num_init_pieces / num_pieces_per_box;
}


double Model::NUM_SOLD_BOXES() const
{
    const double num_sold_pieces = this->m_NUM_SOLD_PIECES;
    const double num_pieces_per_box = this->m_NUM_PIECES_PER_BOX;
    return num_sold_pieces / num_pieces_per_box;
}


double Model::NUM_LEFT_BOXES() const
{
    const double num_left_pieces = this->NUM_LEFT_PIECES();
    const double num_pieces_per_box = this->m_NUM_PIECES_PER_BOX;

    return num_left_pieces / num_pieces_per_box;
}


// conver the number of pieces to the number of boxes
double Model::num_pieces_2_num_boxes(unsigned long n) const
{
    const double num_pieces = n;
    const double num_pieces_per_box = this->m_NUM_PIECES_PER_BOX;

    return num_pieces / num_pieces_per_box;
}


// return the prize of the corresponding number of pieces
double Model::TOTAL_PRIZE(unsigned long num_pieces) const {
    const double num_items_temp = num_pieces;
    return this->m_PRICE * num_items_temp;
}


// sell num_pieces pieces
// check if the number is valid, return true if we have enough left pieces
bool Model::Sell(unsigned long num_pieces_to_sell)
{
    // we make sure we have enough left piece to sell
    if(this->NUM_LEFT_PIECES() >= num_pieces_to_sell){
        this->m_NUM_SOLD_PIECES += num_pieces_to_sell;
        return true;
    }

    return false;
}


// add back this many pieces
// check if the number is valid, return true if they don't exceed num_sold_pieces
bool Model::AddBack(unsigned long num_pieces_to_addBack)
{
    if(num_pieces_to_addBack <= this->m_NUM_SOLD_PIECES){
        this->m_NUM_SOLD_PIECES -= num_pieces_to_addBack;
        return true;
    }

    return false;
}


/* return items that are used to put in the search result table */
void Model::searchResult_Regular(QVector<QString> &items) const
{
    items.clear();
    items.reserve(13);

    items.push_back(this->m_MODEL_CODE); // 1.货号

    if( this->m_Container.isNull() ) // 2.集装箱号
        items.push_back("");
    else
        items.push_back(this->m_Container->ID);

    items.push_back(this->m_DESCRIPTION_CN); // 3.品名中文
    items.push_back(this->m_DESCRIPTION_SPAN); // 4.品名西语
    items.push_back(GlobalVars::locale.toString(this->num_pieces_2_num_boxes(this->m_NUM_INIT_PIECES), 'f', 2)); // 5.进货箱数
    items.push_back(GlobalVars::locale.toString(this->num_pieces_2_num_boxes(this->m_NUM_SOLD_PIECES), 'f', 2)); // 6.已售箱数
    items.push_back(GlobalVars::locale.toString(this->num_pieces_2_num_boxes(this->NUM_LEFT_PIECES()), 'f', 2)); // 7.剩余箱数

    items.push_back(GlobalVars::locale.toString(this->m_NUM_PIECES_PER_BOX)); // 8.每箱几个

    items.push_back(GlobalVars::locale.toString(this->m_NUM_INIT_PIECES)); // 9.进货个数
    items.push_back(GlobalVars::locale.toString(this->m_NUM_SOLD_PIECES)); // 10.已售个数
    items.push_back(GlobalVars::locale.toString(this->NUM_LEFT_PIECES())); // 11.剩余个数

    items.push_back(GlobalVars::locale.toString(this->m_PRICE, 'f', 2)); // 12.单价

    if(this->m_last_time_modified.isNull()){
        write_error_file("m_last_time_modified is NULL");
    }
    else items.push_back(this->m_last_time_modified->toString(GlobalVars::DateTimeFormat)); // 13. 修改时间

    if(this->m_last_time_modified.isNull()){
        write_error_file("m_time_created is NULL");
    }
    else items.push_back(this->m_time_created->toString(GlobalVars::DateTimeFormat)); // 13. 创建时间/进货时间

}


// need to call qDebug().noquote() to print this message
QString Model::describe_this_model() const
{
    QString ret = this->m_MODEL_CODE + "\n"
                  + this->m_DESCRIPTION_CN + "/n"
                  + this->m_DESCRIPTION_SPAN + "\n"
                  + QString::number(this->m_PRICE) + "\n"
                  + QString::number(this->m_NUM_INIT_PIECES) + "\n"
                  + QString::number(this->m_NUM_SOLD_PIECES) + "\n"
                  + QString::number(this->NUM_LEFT_PIECES()) + "\n"
                  + QString::number(this->m_NUM_PIECES_PER_BOX) + "\n";
    if(!this->m_last_time_modified.isNull()){
        ret += this->m_last_time_modified->toString(GlobalVars::DateTimeFormat);
    }
    if(!this->m_time_created.isNull()){
        ret += this->m_time_created->toString(GlobalVars::DateTimeFormat);
    }

    return ret;
}
