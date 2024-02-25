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
private:
    // 尤其需要注意的是，可能有多个货物有一样的货号，但是他们必须有不同的集装箱号
    QString MODEL_CODE;
    QString DESCRIPTION_SPAN; // 西班牙语品名
    QString DESCRIPTION_CN; // 中文品名
    double PRIZE; // 每个单价
    unsigned long NUM_INIT_PIECES; // 初始个数
    unsigned long NUM_SOLD_PIECES; // 卖出个数
    unsigned long NUM_LEFT_PIECES; // 剩余总个数
    unsigned int NUM_PIECES_PER_BOX; //每箱多少个

public:
    QSharedPointer<Container> container; // 从哪个集装箱运过来的

    Model();
    Model(const QString& MODEL_CODE, const QString& DESCRIPTION_SPAN, const QString& DESCRIPTION_CN,
          const double& PRIZE, const unsigned long& NUM_INIT_PIECES, const unsigned long& NUM_SOLD_PIECES,
          const unsigned long& NUM_LEFT_PIECES, const unsigned int& NUM_PIECES_PER_BOX);
    ~Model();

    void reset();

    double TOTAL_PRIZE(const long num_items) const;
    double num_boxes(const long num_items) const;
    
    bool sell(const unsigned long num_pieces);
    bool addBack(const unsigned long num_pieces_2_add_back)
    void addBack_boxes(double num_boxes);
    
    void searchResult_Regular(QVector<QString>& items) const;
    void searchResult_List(QVector<QString> &items) const;

    QString describe_this_model() const;
};

#endif // MODEL_H
