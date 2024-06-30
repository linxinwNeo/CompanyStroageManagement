#include "Model.h"
#include "DataStructures/Container.h"
#include "GlobalVars.h"
#include "qdebug.h"

Model::Model()
{
    this->reset();
}

Model::Model(const QString &MODEL_CODE, const QString &DESCRIPTION_SPAN, const QString &DESCRIPTION_CN, const double &PRIZE,
             const unsigned long &NUM_INIT_PIECES, const unsigned long &NUM_SOLD_PIECES,
             const unsigned long &NUM_PIECES_PER_BOX)
{
    this->MODEL_CODE = MODEL_CODE;
    this->DESCRIPTION_SPAN = DESCRIPTION_SPAN;
    this->DESCRIPTION_CN = DESCRIPTION_CN;
    this->PRIZE = PRIZE;

    this->NUM_INIT_PIECES = NUM_INIT_PIECES;
    this->NUM_SOLD_PIECES = NUM_SOLD_PIECES;

    this->NUM_PIECES_PER_BOX = NUM_PIECES_PER_BOX;

    this->last_time_modified = QSharedPointer<QDateTime>::create(QDateTime().currentDateTime());
    this->container = nullptr;
}


Model::~Model()
{
    this->reset();
}


void Model::reset()
{
    MODEL_CODE = "";
    DESCRIPTION_CN = "";
    DESCRIPTION_SPAN = "";
    PRIZE = 0.;
    NUM_INIT_PIECES = 0;
    NUM_SOLD_PIECES = 0;
    NUM_PIECES_PER_BOX = 0;

    this->container = nullptr;
    this->last_time_modified = nullptr;
}


unsigned long Model::NUM_LEFT_PIECES() const
{
    return this->NUM_INIT_PIECES - this->NUM_SOLD_PIECES;
}


double Model::num_init_boxes() const
{
    const double num_init_pieces = this->NUM_INIT_PIECES;
    const double num_pieces_per_box = this->NUM_PIECES_PER_BOX;
    return num_init_pieces / num_pieces_per_box;
}


double Model::num_sold_boxes() const
{
    const double num_sold_pieces = this->NUM_SOLD_PIECES;
    const double num_pieces_per_box = this->NUM_PIECES_PER_BOX;
    return num_sold_pieces / num_pieces_per_box;
}


double Model::num_left_boxes() const
{
    const double num_left_pieces = this->NUM_LEFT_PIECES();
    const double num_pieces_per_box = this->NUM_PIECES_PER_BOX;

    return num_left_pieces / num_pieces_per_box;
}


// conver the number of pieces to the number of boxes
double Model::num_pieces_2_num_boxes(unsigned long n) const
{
    const double num_pieces = n;
    const double num_pieces_per_box = this->NUM_PIECES_PER_BOX;

    return num_pieces / num_pieces_per_box;
}


// return the prize of the corresponding number of pieces
double Model::TOTAL_PRIZE(unsigned long num_pieces) const {
    const double num_items_temp = num_pieces;
    return this->PRIZE * num_items_temp;
}


// sell num_pieces pieces
// check if the number is valid, return true if we have enough left pieces
bool Model::Sell(unsigned long num_pieces_to_sell)
{
    // we make sure we have enough left piece to sell
    if(this->NUM_LEFT_PIECES() >= num_pieces_to_sell){
        this->NUM_SOLD_PIECES += num_pieces_to_sell;
        return true;
    }

    return false;
}


// add back this many pieces
// check if the number is valid, return true if they don't exceed num_sold_pieces
bool Model::AddBack(unsigned long num_pieces_to_addBack)
{
    if(num_pieces_to_addBack <= this->NUM_SOLD_PIECES){
        this->NUM_SOLD_PIECES -= num_pieces_to_addBack;
        return true;
    }

    return false;
}


/* return items that are used to put in the search result table */
void Model::searchResult_Regular(QVector<QString> &items) const
{
    items.clear();
    items.reserve(13);

    items.push_back(this->MODEL_CODE); // 1.货号

    if( this->container.isNull() ) // 2.集装箱号
        items.push_back("");
    else
        items.push_back(this->container->ID);

    items.push_back(this->DESCRIPTION_CN); // 3.品名中文
    items.push_back(this->DESCRIPTION_SPAN); // 4.品名西语
    items.push_back(locale.toString(this->num_pieces_2_num_boxes(this->NUM_INIT_PIECES), 'f', 2)); // 5.进货箱数
    items.push_back(locale.toString(this->num_pieces_2_num_boxes(this->NUM_SOLD_PIECES), 'f', 2)); // 6.已售箱数
    items.push_back(locale.toString(this->num_pieces_2_num_boxes(this->NUM_LEFT_PIECES()), 'f', 2)); // 7.剩余箱数

    items.push_back(locale.toString(this->NUM_PIECES_PER_BOX)); // 8.每箱几个

    items.push_back(locale.toString(this->NUM_INIT_PIECES)); // 9.进货个数
    items.push_back(locale.toString(this->NUM_SOLD_PIECES)); // 10.已售个数
    items.push_back(locale.toString(this->NUM_LEFT_PIECES())); // 11.剩余个数

    items.push_back(locale.toString(this->PRIZE, 'f', 2)); // 12.单价

    items.push_back(this->last_time_modified->toString(DateTimeFormat)); // 13. 修改时间
}


// need to call qDebug().noquote() to print this message
QString Model::describe_this_model() const
{
    QString ret = this->MODEL_CODE + "\n"
                  + this->DESCRIPTION_CN + "/n"
                  + this->DESCRIPTION_SPAN + "\n"
                  + QString::number(this->PRIZE) + "\n"
                  + QString::number(this->NUM_INIT_PIECES) + "\n"
                  + QString::number(this->NUM_SOLD_PIECES) + "\n"
                  + QString::number(this->NUM_LEFT_PIECES()) + "\n"
                  + QString::number(this->NUM_PIECES_PER_BOX) + "\n";
    if(!this->last_time_modified.isNull()){
        ret += this->last_time_modified->toString(DateTimeFormat);
    }

    return ret;
}
