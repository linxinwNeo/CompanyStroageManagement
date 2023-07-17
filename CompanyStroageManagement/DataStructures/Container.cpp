#include "Container.h"
#include "DataStructures/Model.h"

Container::Container()
{
    this->ID = "";
    this->models.reserve(100);
}

Container::Container(const QString& id)
{
    this->ID = id;
    this->models.reserve(100);
}


Container::~Container()
{
    this->ID = "";
    this->models.clear();
}


void Container::add_model(QSharedPointer<Model> &m)
{
    this->models.push_back(m);
}


UI Container::num_models() const
{
    return this->models.size();
}


double Container::total_num_init_boxes() const
{
    double sum = 0;
    for(const auto& m : this->models){
        sum += m->NUM_INIT_BOXES;
    }
    return sum;
}


double Container::total_num_left_boxes() const
{
    double sum = 0;
    for(const auto& m : this->models){
        sum += m->NUM_LEFT_BOXES;
    }
    return sum;
}

