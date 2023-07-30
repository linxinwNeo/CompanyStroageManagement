#ifndef CONTAINER_H
#define CONTAINER_H

#include "Predefined.h"
#include <QString>
#include <QSet>
#include <QSharedPointer>

#define ContainerPtr QSharedPointer<Container>

// one container contains many models and these models are unique in this container
// some models may exist in other containers and have the the same <MODEL_CODE>

class Model;

class Container {
public:
    QString ID; // the combination of characters and numbers
    QSet< QSharedPointer<Model> > models_set;

    Container();
    Container(const QString& ID);
    ~Container();

    void add_model(QSharedPointer<Model>& m);
    void remove_model(QSharedPointer<Model>& m);

    bool has_model(QSharedPointer<Model>& m) const;

    UI num_models() const;

    double total_num_init_boxes() const;
    double total_num_left_boxes() const;

    double total_num_init_items() const;
    double total_num_left_items() const;

    void searchResult(QVector<QString>& items) const;

    void models_Set2Vec(QVector<QSharedPointer<Model> >& output, const bool sort);
};

#endif // CONTAINER_H
