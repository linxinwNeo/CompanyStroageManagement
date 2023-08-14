#ifndef MODEL_H
#define MODEL_H

#include "Predefined.h"
#include <QString>
#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include <QSet>

#define ModelPtr QSharedPointer<Model>

class Container;

class Model
{
public:
    QSharedPointer<Container> container; // 从哪个集装箱运过来的

    QString MODEL_CODE; // multiple models may have the same model_code but they do not have the same container
    QString DESCRIPTION_SPAN; // 西班牙语品名
    QString DESCRIPTION_CN; // 中文品名
    double PRIZE; // 每件单价
    double NUM_INIT_BOXES; // 初始箱数
    double NUM_SOLD_BOXES; // 卖出箱数
    double NUM_LEFT_BOXES; // 剩余总箱数
    long int NUM_LEFT_ITEMS; // 剩余总件数
    long int NUM_ITEMS_PER_BOX; //多少件每箱

    Model();
    Model(const QString& MODEL_CODE, const QString& DESCRIPTION_SPAN, const QString& DESCRIPTION_CN,
          const double& PRIZE, const double& NUM_INIT_BOXES, const double& NUM_SOLD_BOXES, const double& NUM_LEFT_BOXES,
          const long int& NUM_LEFT_ITEMS, const long int& NUM_ITEMS_PER_BOX);
    ~Model();

    void reset();

    double TOTAL_PRIZE(UI num_items) const;
    double TOTAL_PRIZE(double num_boxes) const;

    void deduct_items(UI num_items);
    void deduct_boxes(double num_boxes);
    void addBack_items(UI num_items);
    void addBack_boxes(double num_boxes);
    
    void searchResult_Regular(QVector<QString>& items) const;
    void searchResult_List(QVector<QString> &items) const;

    QString describe_this_model() const;
};

#endif // MODEL_H
