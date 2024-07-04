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
    QSharedPointer<Container> m_Container; // 从哪个集装箱运过来的

    QString m_MODEL_CODE; // multiple models may have the same model_code but they do not have the same container
    QString m_DESCRIPTION_SPAN; // 西班牙语品名
    QString m_DESCRIPTION_CN; // 中文品名
    double m_PRICE; // 每个单价
    unsigned long m_NUM_INIT_PIECES; // 初始个数
    unsigned long m_NUM_SOLD_PIECES; // 卖出个数
    unsigned long m_NUM_PIECES_PER_BOX; //每箱多少个

    QSharedPointer<QDateTime> m_last_time_modified; // 上次被修改的时间
    QSharedPointer<QDateTime> m_time_created; // 上次被修改的时间

    Model();
    Model(const QString& MODEL_CODE,
          const QString& DESCRIPTION_SPAN,
          const QString& DESCRIPTION_CN,
          const double& PRIZE,
          const unsigned long& NUM_INIT_PIECES,
          const unsigned long& NUM_SOLD_PIECES,
          const unsigned long& NUM_PIECES_PER_BOX);
    ~Model();

    void reset();

    unsigned long NUM_LEFT_PIECES() const;
    
    double NUM_INIT_BOXES() const;
    double NUM_SOLD_BOXES() const;
    double NUM_LEFT_BOXES() const;
    
    double num_pieces_2_num_boxes(unsigned long num_pieces) const;
    double num_pieces_2_num_boxes(float) const = delete; // Prevent calling with float
    double num_pieces_2_num_boxes(double) const = delete; // Prevent calling with double
    double num_pieces_2_num_boxes(long double) const = delete; // Prevent calling with long double

    double TOTAL_PRIZE(unsigned long num_pieces) const;
    bool TOTAL_PRIZE(float) const = delete; // Prevent calling with float
    bool TOTAL_PRIZE(double) const = delete; // Prevent calling with double
    bool TOTAL_PRIZE(long double) const = delete; // Prevent calling with long double
    
    bool Sell(unsigned long num_pieces_to_sell);
    bool Sell(float) = delete; // Prevent calling with float
    bool Sell(double) = delete; // Prevent calling with double
    bool Sell(long double) = delete; // Prevent calling with long double

    bool AddBack(unsigned long num_pieces_to_addBack);
    bool AddBack(float) = delete; // Prevent calling with float
    bool AddBack(double) = delete; // Prevent calling with double
    bool AddBack(long double) = delete; // Prevent calling with long double
    
    void searchResult_Regular(QVector<QString>& items) const;

    QString describe_this_model() const;
};

#endif // MODEL_H
