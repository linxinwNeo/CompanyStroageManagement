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

    // model stuff
    void add_model( ModelPtr& m );
    QSet<ModelPtr> get_Model(const QString& MODEL_CODE);
    bool contains_model(const ModelPtr& m) const;
    bool contains_model(const QString& MODEL_CODE) const;

    // container stuff
    void add_container( ContainerPtr& container );
    ContainerPtr get_container(const QString& ID);
    bool contains_container(const QString& ID) const;
    bool contains_container(const ContainerPtr& container) const;

    UI num_models() const;
    UI num_containers() const;

    void searchModel_starts_with(const QString str,  QVector<ModelPtr>& models);
    void searchContainer_starts_with(const QString str,  QVector<ContainerPtr>& containers);
};


#endif // INVENTORY_H
