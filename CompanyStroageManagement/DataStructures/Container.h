#ifndef CONTAINER_H
#define CONTAINER_H

#include "Predefined.h"
#include <QString>
#include <QVector>
#include <QSharedPointer>

#define ContainerPtr QSharedPointer<Container>

// one container contains many models and these models are unique in this container
// some models may exist in other containers and have the the same <MODEL_CODE>

class Model;

class Container {
public:
    QString ID; // the combination of characters and numbers
    QVector<QSharedPointer<Model>> models;

    Container();
    Container(const QString& ID);
    ~Container();

    void add_model(QSharedPointer<Model>& m);

    UI num_models() const;

    double total_num_init_boxes() const;
    double total_num_left_boxes() const;
};

#endif // CONTAINER_H
