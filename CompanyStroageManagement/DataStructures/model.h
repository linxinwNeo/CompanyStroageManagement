#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include <QSet>
#include <QDateTime>

#define ModelPtr QSharedPointer<Model>

class Container;

class Model
{
public:
    QSharedPointer<Container> container; // 从哪个集装箱运过来的

    QString MODEL_CODE; // multiple models may have the same model_code but they do not have the same container
    QString DESCRIPTION_SPAN; // 西班牙语品名
    QString DESCRIPTION_CN; // 中文品名
    double PRIZE; // 每个单价
    unsigned long NUM_INIT_PIECES; // 初始个数
    unsigned long NUM_SOLD_PIECES; // 卖出个数
    unsigned long NUM_LEFT_PIECES; // 剩余总个数
    unsigned long NUM_PIECES_PER_BOX; //多少个每箱

    QSharedPointer<QDateTime> last_time_modified; // 上次被修改的时间

    Model();
    Model(const QString& MODEL_CODE, const QString& DESCRIPTION_SPAN, const QString& DESCRIPTION_CN,
          const double& PRIZE,
          const unsigned long& NUM_INIT_PIECES, const unsigned long& NUM_SOLD_PIECES, const unsigned long& NUM_LEFT_PIECES,
          const unsigned long& NUM_PIECES_PER_BOX);
    ~Model();

    void reset();

    double init_num_boxes() const;
    double sold_num_boxes() const;
    double left_num_boxes() const;

    double TOTAL_PRIZE(unsigned long num_items) const;
    
    bool Sell(unsigned long num_pieces_to_sell);
    bool AddBack(unsigned long num_pieces_to_addBack);
    
    void searchResult_Regular(QVector<QString>& items) const;

    QString describe_this_model() const;
};

#endif // MODEL_H
