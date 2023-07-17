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
    this->model_map.clear();
    this->model_set.clear();

    this->container_map.clear();
    this->container_set.clear();
}


void Inventory::clear()
{
    this->reset();
}


void Inventory::add_model(ModelPtr &m)
{
    if(this->contains_model(m)) return;

    // put this model to the set
    this->model_set.insert(m);

    // put this model to the map, but map may or may not have the entry defined, we need to check
    if(this->model_map.contains(m->MODEL_CODE)){
        this->model_map[m->MODEL_CODE].insert(m);
    }
    else{ // make a set for the entry
        QSet<ModelPtr> set = {m};
        this->model_map[m->MODEL_CODE] = set;
    }

    return;
}


// fast O(1) search
QSet<ModelPtr> Inventory::get_Model(const QString &MODEL_CODE)
{
    // if the model_map has the corresponding entry defined, we just return what's in that entry
    if(this->model_map.contains(MODEL_CODE)) return this->model_map[MODEL_CODE];

    // if not, we return empty set
    QSet<ModelPtr> emptySet;
    return emptySet;
}


// fast, O(1)
bool Inventory::contains_model(const QSharedPointer<Model> &m) const
{
    return this->model_set.contains(m);
}


// fast, O(1)
bool Inventory::contains_model(const QString &MODEL_CODE) const
{
    return this->model_map.contains(MODEL_CODE);
}



// add a container to the inventory, it is possible that the container is a duplicate
void Inventory::add_container(QSharedPointer<Container> &container)
{
    if(this->contains_container(container)) return;

    this->container_map[container->ID] = container;
    this->container_set.insert(container);
}


QSharedPointer<Container> Inventory::get_container(const QString &ID)
{
    return this->container_map[ID];
}


bool Inventory::contains_container(const QString &containerID) const
{
    return this->container_map.contains(containerID);
}


bool Inventory::contains_container(const QSharedPointer<Container> &container) const
{
    return this->container_set.contains(container);
}


UI Inventory::num_models() const
{
    return this->model_set.size();
}


UI Inventory::num_containers() const
{
    return this->container_set.size();
}


/* search the inventory for the Model.modelcode that is starting with <str>, add to  <models>*/
void Inventory::searchModel_starts_with(const QString str, QVector<ModelPtr>& models)
{
    models.reserve(this->num_models());

    for(const ModelPtr m : this->model_set){
        if(m->MODEL_CODE.startsWith(str)){
            models.push_back(m);
        }
    }

    return;
}


void Inventory::searchContainer_starts_with(const QString str, QVector< ContainerPtr > &containers)
{
    containers.reserve(this->num_containers());

    for(const ContainerPtr c : this->container_set){
        if(c->ID.startsWith(str)){
            containers.push_back(c);
        }
    }

    return;
}

