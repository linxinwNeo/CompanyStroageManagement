#ifndef STROAGE_H
#define STROAGE_H

#include <QString>
#include <QHash>
#include <DataStructures/model.h>

class Stroage
{
private:
    QHash< QString, QSharedPointer<Model> > model_list;

public:
    Stroage();
    ~Stroage();

    void reset();
    bool add_model( QSharedPointer<Model> );
    UI size() const { return this->model_list.size(); }

    QSharedPointer<Model> search(const QString); // it is possible to return nullptr

    bool deduct();
    bool deduct(QSharedPointer<Model>, double num) const;
    bool deduct(const QString, double num);

    bool addBack();
    bool addBack(QSharedPointer<Model>, double num) const;
    bool addBack(const QString, double num);

    void modify_excel(QSharedPointer<Model>) const;

    void readDB() const;
    void save2DB() const;

    const QHash< QString, QSharedPointer<Model> >& get_model_list() const {return this->model_list;};

};


#endif // STROAGE_H
