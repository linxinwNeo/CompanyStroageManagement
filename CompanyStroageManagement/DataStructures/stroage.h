#ifndef STROAGE_H
#define STROAGE_H

#include <QString>
#include <unordered_map>
#include <DataStructures/model.h>

class Stroage
{
private:
    std::unordered_map<QString, Model*> model_list;

public:
    Stroage();
    ~Stroage();

    void reset();
    bool add_model(Model* );
    UI size() const { return this->model_list.size(); }

    Model* search(const QString); // it is possible to return nullptr

    bool deduct();
    bool deduct(Model*, double num) const;
    bool deduct(const QString, double num);

    bool addBack();
    bool addBack(Model*, double num) const;
    bool addBack(const QString, double num);

    void modify_excel(Model*) const;

    void readDB() const;
    void save2DB() const;

    const std::unordered_map<QString, Model*> get_model_list() const {return this->model_list;};

};


#endif // STROAGE_H
