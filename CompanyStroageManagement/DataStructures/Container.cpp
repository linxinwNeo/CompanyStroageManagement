#include "Container.h"
#include "Algorithm/QuickSort.h"
#include "DataStructures/Model.h"
#include "qdebug.h"

Container::Container()
{
    this->ID = "";
    this->models_set.reserve(100);
}


Container::Container(const QString& id)
{
    this->ID = id;
    this->models_set.reserve(100);
}


Container::~Container()
{
    this->ID = "";
    this->models_set.clear();
}


// since models is a set, we will not have duplicates
void Container::add_model(ModelPtr &m)
{
    this->models_set.insert(m);
}


void Container::remove_model(ModelPtr &m)
{
    if(!this->models_set.remove(m)){
        qDebug() << "Container::remove_model: remove model failed";
    }
}


bool Container::has_model(ModelPtr &m) const
{
    return this->models_set.contains(m);
}


UI Container::num_models() const
{
    return this->models_set.size();
}


double Container::total_num_init_boxes() const
{
    double sum = 0;
    for(const auto& m : this->models_set){
        sum += m->NUM_INIT_BOXES;
    }
    return sum;
}


double Container::total_num_left_boxes() const
{
    double sum = 0;
    for(const auto& m : this->models_set){
        sum += m->NUM_LEFT_BOXES;
    }
    return sum;
}


// 计算初始总个数
double Container::total_num_init_items() const
{
    double sum = 0;
    for(const auto& m : this->models_set){
        sum += (m->NUM_INIT_BOXES * m->NUM_ITEMS_PER_BOX);
    }

    return sum;
}


// 计算剩余总个数
double Container::total_num_left_items() const
{
    double sum = 0;
    for(const auto& m : this->models_set){
        sum += m->NUM_LEFT_ITEMS;
    }

    return sum;
}


// return a list of string that describes this item in the search_container_table
void Container::searchResult(QVector<QString> &items) const
{
    items.clear();
    items.reserve(6);

    items.push_back(this->ID); // 集装箱号
    items.push_back(QString::number(this->num_models())); // 货物种类数量
    items.push_back(QString::number(this->total_num_init_boxes(), 'f', 2)); // 初始总箱数
    items.push_back(QString::number(this->total_num_left_boxes(), 'f', 2)); // 剩余总箱数

    items.push_back(QString::number(this->total_num_init_items(), 'f', 2)); // 初始总个数
    items.push_back(QString::number(this->total_num_left_items(), 'f', 2)); // 剩余总个数
}


// return vector of models
void Container::models_Set2Vec(QVector<QSharedPointer<Model> >& output, const bool sort)
{
    output.clear();
    output.reserve(this->num_models());
    for(auto& model : this->models_set){
        output.push_back(model);
    }

    QuickSorts QS;
    if(sort) QS.QuickSort(output);
}

