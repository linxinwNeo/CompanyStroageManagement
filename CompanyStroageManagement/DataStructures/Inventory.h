#ifndef INVENTORY_H
#define INVENTORY_H

#include <QString>
#include <QHash>
#include <DataStructures/Model.h>
#include <DataStructures/Container.h>

/* models can have the same MODEL_CODE (because they are from different container)
 */
class Inventory
{
public:
    QHash< QString, QSet<ModelPtr> > model_map; // a key may give multiple models froms different containers
    QSet<ModelPtr> model_set; // holds all models

    QHash< QString, ContainerPtr > container_map; // key is the container ID
    QSet<ContainerPtr> container_set; // holds all models

    Inventory();
    ~Inventory();

    void reset();
    void clear();
    void reserve_model_space(UI size);
    void reserve_container_space(UI size);

    // model stuff
    void add_Model( ModelPtr& m );
    QSet<ModelPtr> get_Model(const QString& MODEL_CODE);
    ModelPtr get_Model(const QString& MODEL_CODE, const QString& Container_ID);

    bool contains_model(const ModelPtr& m) const;
    bool contains_model(const QString& MODEL_CODE) const;

    // container stuff
    void add_Container( ContainerPtr& container );
    ContainerPtr get_container(const QString& ID);
    bool contains_container(const QString& ID) const;
    bool contains_container(const ContainerPtr& container) const;

    UI num_models() const;
    UI num_containers() const;

    void searchModel_starts_with(const QString str,  QVector<ModelPtr>& models);
    void searchContainer_starts_with(const QString str,  QVector<ContainerPtr>& containers);
};


#endif // INVENTORY_H
