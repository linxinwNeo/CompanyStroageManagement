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
    QSet<ModelPtr> model_set;
    QVector<ModelPtr> model_vec;

    QHash<QString, ContainerPtr> container_map;
    QVector<ContainerPtr> container_vec;

    Inventory();
    ~Inventory();

    void reset();
    void clear();

    // model stuff
    void add( ModelPtr& m );
    ModelPtr get_Model(const QString& MODEL_CODE);
    bool contains_model(const ModelPtr& m) const;
    bool contains_model(const QString& MODEL_CODE) const;

    UI models_size() const;

    // container stuff
    void add( ContainerPtr& container );
    ContainerPtr get_container(const QString& ID);
    bool contains_container(const QString& ID) const;
    bool contains_container(const ContainerPtr& container) const;

    UI containers_size() const;

    void searchModel_starts_with(const QString str,  QVector<ModelPtr>& models);
};


#endif // INVENTORY_H
