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


unsigned long Container::num_models() const
{
    return this->models_set.size();
}


double Container::total_num_init_boxes() const
{
    double sum = 0;
    for(const auto& m : this->models_set){
        sum += m->num_init_boxes();
    }
    return sum;
}


double Container::total_num_left_boxes() const
{
    double sum = 0;
    for(const auto& m : this->models_set){
        sum += m->num_left_boxes();
    }
    return sum;
}


// 计算进货总个数
unsigned long long Container::total_num_init_pieces() const
{
    unsigned long long sum = 0;
    for(const auto& m : this->models_set){
        sum += m->NUM_INIT_PIECES;
    }

    return sum;
}


// 计算剩余总个数
unsigned long long Container::total_num_left_pieces() const
{
    unsigned long long sum = 0;
    for(const auto& m : this->models_set){
        sum += m->NUM_LEFT_PIECES;
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

    items.push_back(QString::number(this->total_num_init_pieces(), 'f', 2)); // 初始总个数
    items.push_back(QString::number(this->total_num_left_pieces(), 'f', 2)); // 剩余总个数
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

