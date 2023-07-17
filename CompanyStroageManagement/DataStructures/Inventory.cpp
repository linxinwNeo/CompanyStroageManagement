#include "Inventory.h"
#include "GlobalVars.h"

Inventory::Inventory()
{
    this->reset();
}


Inventory::~Inventory()
{
    this->reset();
}


// clear memory used by model_list
void Inventory::reset()
{
    this->container_map.clear();
    this->container_vec.clear();
    this->model_set.clear();
    this->model_vec.clear();

    this->container_vec.reserve(100);
    this->model_vec.reserve(50000);
}


void Inventory::clear()
{
    this->reset();
}


void Inventory::add(QSharedPointer<Model> &m)
{
    if(this->contains_model(m)){
        return;
    }

    this->model_set.insert(m);
    this->model_vec.push_back(m);
}


// slow O(n) search
QSharedPointer<Model> Inventory::get_Model(const QString &MODEL_CODE)
{
    for(const auto& m : this->model_vec){
        if(m->MODEL_CODE == MODEL_CODE){
            return m;
        }
    }

    return nullptr;
}


bool Inventory::contains_model(const QSharedPointer<Model> &m) const
{
    return this->model_set.contains(m);
}


bool Inventory::contains_model(const QString &MODEL_CODE) const
{
    for(const auto& m : this->model_vec){
        if(m->MODEL_CODE == MODEL_CODE){
            return true;
        }
    }

    return false;
}

UI Inventory::models_size() const
{
    return this->model_vec.size();
}


void Inventory::add(QSharedPointer<Container> &container)
{
    if(this->contains_container(container)){
        return;
    }

    this->container_map[container->ID] = container;
    this->container_vec.push_back(container);
}


QSharedPointer<Container> Inventory::get_container(const QString &ID)
{
    return this->container_map[ID];
}


bool Inventory::contains_container(const QString &ID) const
{
    return this->container_map.contains(ID);
}


bool Inventory::contains_container(const QSharedPointer<Container> &container) const
{
    for(const auto& c : this->container_vec){
        if(c == container){
            return true;
        }
    }

    return false;
}

UI Inventory::containers_size() const
{
    return this->container_vec.size();
}


UI Inventory::num_models() const
{
    return this->model_vec.size();
}


UI Inventory::num_containers() const
{
    return this->container_vec.size();
}


/* search the inventory for the Model.modelcode that is starting with <str>, add to  <models>*/
void Inventory::searchModel_starts_with(const QString str, QVector<ModelPtr>& models)
{
    models.reserve(this->num_models());

    for(const ModelPtr m : this->model_vec){
        if(m->MODEL_CODE.startsWith(str)){
            models.push_back(m);
        }
    }

    return;
}




