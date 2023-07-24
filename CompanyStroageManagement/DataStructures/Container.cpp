#include "Container.h"
#include "DataStructures/Model.h"
#include "qdebug.h"

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


// since models is a set, we will not have duplicates
void Container::add_model(ModelPtr &m)
{
    this->models.insert(m);
}


void Container::remove_model(ModelPtr &m)
{
    if(!this->models.remove(m)){
        qDebug() << "Container::remove_model: remove model failed";
    }
}


bool Container::has_model(ModelPtr &m) const
{
    return this->models.contains(m);
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

