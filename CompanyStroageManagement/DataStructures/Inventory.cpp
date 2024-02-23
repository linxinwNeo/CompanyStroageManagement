#include "Inventory.h"
#include "Algorithm/QuickSort.h"
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


void Inventory::reserve_model_space(unsigned int size)
{
    // return if model containers are not empty
    if(this->model_map.size() != 0 || this->model_set.size() != 0) return;

    this->model_map.reserve(size);
    this->model_set.reserve(size);
}


void Inventory::reserve_container_space(unsigned int size)
{
    // return if container containers are not empty
    if(this->container_map.size() != 0 || this->container_set.size() != 0) return;

    this->container_map.reserve(size);
    this->container_set.reserve(size);
}


void Inventory::add_Model(ModelPtr &m)
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


// remove the model, including all containers that reference it
void Inventory::remove_Model(QSharedPointer<Model> &m)
{
    // remove this model from inventory
    this->model_set.remove(m);

    QSet<ModelPtr>& set = this->model_map[m->MODEL_CODE];
    set.remove(m);


    // remove this model from the container it belongs to
    ContainerPtr container = m->container;
    if(container.isNull()) return;
    container->remove_model(m);
}


// fast O(1) search
QSet<ModelPtr> Inventory::get_Model(const QString &MODEL_CODE)
{
    // if the model_map has the corresponding entry defined, we just return what's in that entry
    if( this->model_map.contains(MODEL_CODE) )
        return this->model_map[MODEL_CODE];

    // if not, we return empty set
    QSet<ModelPtr> emptySet;
    return emptySet;
}


// get the model with the container
// if the mode does not have a container, <Container_ID> is supposed to be an empty string
QSharedPointer<Model> Inventory::get_Model(const QString &MODEL_CODE, const QString &Container_ID)
{
    QSet<ModelPtr> candidates = this->get_Model(MODEL_CODE);
    for(ModelPtr m : candidates){
        if(m->container.isNull() && Container_ID.isEmpty()){
            return m;
        }
        else if(!m->container.isNull() && m->container->ID == Container_ID){
            return m;
        }
    }

    return nullptr;
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
void Inventory::add_Container(QSharedPointer<Container> &container)
{
    if(this->contains_container(container)) return;

    this->container_map[container->ID] = container;
    this->container_set.insert(container);
}


QSharedPointer<Container> Inventory::get_container(const QString &ID)
{
    if(ID.trimmed().isEmpty()) return nullptr;

    if(this->contains_container(ID)){
        return this->container_map[ID];
    }

    return nullptr;
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


/* search the inventory for the Model.modelcode that is starting with <str>, add to  <models>
   then we need to sort the <models> by MODELCODEs in alphabetical order
*/
void Inventory::searchModel_starts_with(const QString str, QVector<ModelPtr>& models)
{
    models.reserve(this->num_models());

    for(const ModelPtr& m : this->model_set){
        if(m->MODEL_CODE.startsWith(str)){
            models.push_back(m);
        }
    }

    QuickSorts QS;
    QS.QuickSort(models);
    return;
}


void Inventory::searchContainer_starts_with(const QString str, QVector< ContainerPtr > &containers)
{
    containers.reserve(this->num_containers());

    for(const ContainerPtr& c : this->container_set){
        if(c->ID.startsWith(str)){
            containers.push_back(c);
        }
    }

    QuickSorts QS;
    QS.QuickSort(containers);
    return;
}


void Inventory::deduct_models(const QVector<EntryPtr> & entries)
{
    for(const EntryPtr& entry : entries){
        const QString& MODELCODE = entry->CLAVE;
        const QString& ContainerID = entry->ContainerID;
        ModelPtr model_needs_modify = this->get_Model(MODELCODE, ContainerID);
        if(model_needs_modify.isNull()) {
            qDebug() << "Could't find the model to deduct!";
            continue;
        }

        model_needs_modify->sell_items(entry->CANTIDAD);
    }
}


// transfer set to vector and sort it
void Inventory::modelSet_2_Vector(QVector<QSharedPointer<Model> >& output)
{
    output.clear();
    output.reserve(this->num_models());

    for(const ModelPtr& model : this->model_set){
        output.push_back(model);
    }

    // sort the vector
    QuickSorts QS;
    QS.QuickSort(output);

}

