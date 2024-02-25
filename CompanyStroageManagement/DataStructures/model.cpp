#include "model.h"
#include "DataStructures/Container.h"
#include "GlobalVars.h"
#include <QDebug>
#include <cmath>

Model::Model()
{
    this->reset();
}


/* constructor with parameters */
Model::Model(const QString &MODEL_CODE, const QString &DESCRIPTION_SPAN, const QString &DESCRIPTION_CN,
             const double &PRIZE, const unsigned long &NUM_INIT_PIECES, const unsigned long &NUM_SOLD_PIECES,
             const unsigned long &NUM_LEFT_PIECES, const unsigned int &NUM_PIECES_PER_BOX)
{
    this->MODEL_CODE = MODEL_CODE;
    this->DESCRIPTION_SPAN = DESCRIPTION_SPAN;
    this->DESCRIPTION_CN = DESCRIPTION_CN;
    this->PRIZE = PRIZE;
    this->NUM_INIT_PIECES = NUM_INIT_PIECES;
    this->NUM_SOLD_PIECES = NUM_SOLD_PIECES;
    this->NUM_LEFT_PIECES = NUM_LEFT_PIECES;
    this->NUM_PIECES_PER_BOX = NUM_PIECES_PER_BOX;

    this->container = nullptr;
}


Model::~Model()
{
    this->reset();
}


/* set variables to the initial state */
void Model::reset()
{
    MODEL_CODE = "";
    DESCRIPTION_CN = "";
    DESCRIPTION_SPAN = "";
    PRIZE = 0.;
    NUM_INIT_PIECES = 0;
    NUM_SOLD_PIECES = 0;
    NUM_LEFT_PIECES = 0;
    NUM_PIECES_PER_BOX = 0;

    this->container = nullptr;
}


// return the total prize of the corresponding items
double Model::TOTAL_PRIZE(const long num_pieces) const {
    return this->PRIZE * ((double)num_pieces);
}


// calculate the number of boxes
double Model::num_boxes(const long num_pieces) const
{
    double num_boxes = num_pieces / ((double)this->NUM_PIECES_PER_BOX);
    double truncated_num = trunc(num_boxes * 100) / 100; // only get first two digits of the double
    return truncated_num;
}


// deduct this many pieces
// return true if sell operation is successful
bool Model::sell(const unsigned long num_pieces_2_sell)
{
    // check the num of left pieces, if not enough, return false
    if(this->NUM_LEFT_PIECES < num_pieces_2_sell){
        return false;
    }

    // sell pieces
    this->NUM_LEFT_PIECES -= num_pieces_2_sell;
    this->NUM_SOLD_PIECES += num_pieces_2_sell;

    return true;
}


// add back this many items
// return false if exceeds sold number of pieces
// return true if addBack opeartion is successful
bool Model::addBack(const unsigned long num_pieces_2_add_back)
{
    if(this->NUM_SOLD_PIECES < num_pieces_2_add_back){
        return false;
    }

    // add back pieces
    this->NUM_LEFT_PIECES += num_pieces_2_add_back;
    this->NUM_SOLD_PIECES -= num_pieces_2_add_back;

    return true;
}


// add back this many boxes, you don't need to call addBack_items anymore.
void Model::addBack_boxes(double num_boxes_to_addBack)
{
    if(NUM_SOLD_BOXES < num_boxes_to_addBack){
        NUM_SOLD_BOXES = 0;
    }
    else{
        NUM_SOLD_BOXES -= num_boxes_to_addBack;
    }
    NUM_LEFT_BOXES += num_boxes_to_addBack;
    NUM_LEFT_ITEMS += (num_boxes_to_addBack * this->NUM_ITEMS_PER_BOX);

    // make sure after adding back items, the number of initial boxes still makes sense
    if(NUM_INIT_BOXES != NUM_LEFT_BOXES + NUM_SOLD_BOXES){
        NUM_INIT_BOXES = NUM_LEFT_BOXES + NUM_SOLD_BOXES;
    }
}


/* return items that are used to put in the search result table */
void Model::searchResult_Regular(QVector<QString> &items) const
{
    items.clear();
    items.reserve(11);

    items.push_back(this->MODEL_CODE); // 货号
    items.push_back(this->DESCRIPTION_CN); // 品名中文
    items.push_back(this->DESCRIPTION_SPAN); // 品名西语
    items.push_back(QString::number(this->NUM_INIT_BOXES, 'f', 2)); // 初始箱数
    items.push_back(QString::number(this->NUM_ITEMS_PER_BOX)); // 每箱几件
    items.push_back(QString::number(this->PRIZE, 'f', 2)); // 单价
    items.push_back(QString::number(this->NUM_SOLD_BOXES, 'f', 2)); // 卖出箱数
    items.push_back(QString::number(this->NUM_SOLD_BOXES * this->NUM_ITEMS_PER_BOX)); // 卖出件数
    items.push_back(QString::number(this->NUM_LEFT_BOXES, 'f', 2)); // 剩余箱数
    items.push_back(QString::number(this->NUM_LEFT_ITEMS)); // 剩余件数

    if(this->container.isNull()) items.push_back(""); // 集装箱号
    else items.push_back(this->container->ID); // 集装箱号

}


/* return items that are used to put in the added_models_table */
void Model::searchResult_List(QVector<QString> &items) const
{
    items.clear();
    items.reserve(9);

    items.push_back(QString::number(this->NUM_LEFT_BOXES, 'f', 2)); // 剩余箱数
    items.push_back(QString::number(this->NUM_LEFT_ITEMS)); // 剩余件数
    items.push_back(QString::number(this->NUM_ITEMS_PER_BOX)); // 每箱几件
    items.push_back(this->MODEL_CODE); // 货号
    items.push_back(this->DESCRIPTION_SPAN); // 品名西语
    items.push_back(this->DESCRIPTION_CN); // 品名中文
    items.push_back(QString::number(this->PRIZE, 'f', 2)); // 单价
    items.push_back(QString::number(this->PRIZE*this->NUM_LEFT_BOXES*this->NUM_ITEMS_PER_BOX, 'f', 2)); // 总价

    if(this->container.isNull()) items.push_back(""); // 集装箱号
    else items.push_back(this->container->ID); // 集装箱号

}


// need to call qDebug().noquote() to print this message
QString Model::describe_this_model() const
{
    return this->MODEL_CODE + "\n"
           + this->DESCRIPTION_CN + "/n"
           + this->DESCRIPTION_SPAN + "\n"
           + QString::number(this->PRIZE) + "\n"
           + QString::number(this->NUM_INIT_BOXES) + "\n"
           + QString::number(this->NUM_SOLD_BOXES) + "\n"
           + QString::number(this->NUM_LEFT_BOXES) + "\n"
           + QString::number(this->NUM_LEFT_ITEMS) + "\n"
           + QString::number(this->NUM_ITEMS_PER_BOX);
}
