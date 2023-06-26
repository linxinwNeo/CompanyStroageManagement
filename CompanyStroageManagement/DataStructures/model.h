#ifndef MODEL_H
#define MODEL_H

#include "Predefined.h"
#include <QString>
#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include <QSet>

#define ModelPtr QSharedPointer<Model>

class Model;

class Container {
public:
    QString ID;
    QVector<ModelPtr> models;

    double total_num_init_boxes() const;
    double total_num_left_boxes() const;
};


#define ContainerPtr QSharedPointer<Container>

class Model
{
public:
    UI IDX;

    ContainerPtr container;

    QString MODEL_CODE; // multiple models may have the same model_code but they do not have the same idx
    QString DESCRIPTION_SPAN;
    QString DESCRIPTION_CN;
    double PRIZE;
    double NUM_INIT_BOXES;
    double NUM_SOLD_BOXES;
    double NUM_LEFT_BOXES;
    long int NUM_LEFT_ITEMS;
    long int NUM_ITEMS_PER_BOX;

    Model();
    Model(const QString& MODEL_CODE, const QString& DESCRIPTION_SPAN, const QString& DESCRIPTION_CN,
          const double& PRIZE, const double& NUM_INIT_BOXES, const double& NUM_SOLD_BOXES, const double& NUM_LEFT_BOXES,
          const long int& NUM_LEFT_ITEMS, const long int& NUM_ITEMS_PER_BOX);
    ~Model();


    void reset();

    double TOTAL_PRIZE(UI num_items) const;
    void deduct_items(UI num_items);
    void deduct_boxes(double num_boxes);
    void addBack_items(UI num_items);
    void addBack_boxes(double num_boxes);
};

#endif // MODEL_H
