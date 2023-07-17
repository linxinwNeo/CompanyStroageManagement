#include "model.h"
#include "DataStructures/Container.h"

Model::Model()
{
    this->reset();
}

Model::Model( const QString& MODEL_CODE, const QString& DESCRIPTION_SPAN, const QString& DESCRIPTION_CN,
              const double& PRIZE, const double& NUM_INIT_BOXES, const double& NUM_SOLD_BOXES, const double& NUM_LEFT_BOXES,
              const long int& NUM_LEFT_ITEMS, const long int& NUM_ITEMS_PER_BOX)
{
    this->MODEL_CODE = MODEL_CODE;
    this->DESCRIPTION_SPAN = DESCRIPTION_SPAN;
    this->DESCRIPTION_CN = DESCRIPTION_CN;
    this->PRIZE = PRIZE;
    this->NUM_INIT_BOXES = NUM_INIT_BOXES;
    this->NUM_SOLD_BOXES = NUM_SOLD_BOXES;
    this->NUM_LEFT_BOXES = NUM_LEFT_BOXES;
    this->NUM_LEFT_ITEMS = NUM_LEFT_ITEMS;
    this->NUM_ITEMS_PER_BOX = NUM_ITEMS_PER_BOX;

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
    NUM_INIT_BOXES = 0.;
    NUM_SOLD_BOXES = 0.;
    NUM_LEFT_BOXES = 0.;
    NUM_LEFT_ITEMS = 0;
    NUM_ITEMS_PER_BOX = 0;

    this->container = nullptr;
}


// return the prize of the corresponding items
double Model::TOTAL_PRIZE(UI num_items) const {
    return this->PRIZE * (double)num_items;
}


double Model::TOTAL_PRIZE(double num_boxes) const
{
    return this->PRIZE * num_boxes * this->NUM_ITEMS_PER_BOX;
}


// deduct this many items
// note: we need to check if one container no longer
void Model::deduct_items(UI num_items)
{
    double num_boxes_to_deduct = num_items / (double)this->NUM_ITEMS_PER_BOX;
    this->NUM_SOLD_BOXES += num_boxes_to_deduct;
    this->NUM_LEFT_BOXES -= num_boxes_to_deduct;
    this->NUM_LEFT_ITEMS -= num_items;
}


// deduct this many boxes
void Model::deduct_boxes(double num_boxes_to_deduct)
{
    this->NUM_SOLD_BOXES += num_boxes_to_deduct;
    this->NUM_LEFT_BOXES -= num_boxes_to_deduct;
    this->NUM_LEFT_ITEMS -= (num_boxes_to_deduct * this->NUM_ITEMS_PER_BOX);
}


// add back this many items
void Model::addBack_items(UI num_items)
{
    double num_boxes_to_addBack = num_items / (double)this->NUM_ITEMS_PER_BOX;
    this->NUM_SOLD_BOXES -= num_boxes_to_addBack;
    this->NUM_LEFT_BOXES += num_boxes_to_addBack;
    this->NUM_LEFT_ITEMS += num_items;
}


void Model::addBack_boxes(double num_boxes_to_addBack)
{
    this->NUM_SOLD_BOXES -= num_boxes_to_addBack;
    this->NUM_LEFT_BOXES += num_boxes_to_addBack;
    this->NUM_LEFT_ITEMS += (num_boxes_to_addBack * this->NUM_ITEMS_PER_BOX);
}


/* return items that are used to put in the search result table */
void Model::searchResult(QVector<QString> &items) const
{
    items.clear();
    items.reserve(11);

    items.push_back(this->MODEL_CODE); // 货号
    items.push_back(this->DESCRIPTION_CN); // 品名中文
    items.push_back(this->DESCRIPTION_SPAN); // 品名西语
    items.push_back(QString::number(this->NUM_INIT_BOXES)); // 初始箱数
    items.push_back(QString::number(this->NUM_ITEMS_PER_BOX)); // 每箱几件
    items.push_back(QString::number(this->PRIZE)); // 单价
    items.push_back(QString::number(this->NUM_SOLD_BOXES)); // 卖出箱数
    items.push_back(QString::number(this->NUM_SOLD_BOXES * this->NUM_ITEMS_PER_BOX)); // 卖出件数
    items.push_back(QString::number(this->NUM_LEFT_BOXES)); // 剩余箱数
    items.push_back(QString::number(this->NUM_LEFT_BOXES * this->NUM_ITEMS_PER_BOX)); // 剩余件数

    if(this->container.isNull()){
        items.push_back("无"); // 集装箱号
    }
    else{
        items.push_back(this->container->ID); // 集装箱号
    }
}
