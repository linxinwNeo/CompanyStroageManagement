#include "stroage.h"
#include "GlobalVars.h"

Stroage::Stroage()
{

}


Stroage::~Stroage()
{
    if(model_list.size() == 0) return;

    for (auto it : this->model_list)
        delete it.second;

    model_list.clear();
    return;
}


// clear memory used by model_list
void Stroage::reset()
{
    if(model_list.size() == 0) return;

    for (auto it : this->model_list)
        delete it.second;

    model_list.clear();
}


// add the model to the unordered map
bool Stroage::add_model(Model * m)
{
    if( this->search(m->get_MODELO()) != nullptr ){
        return false;
    }
    this->model_list[m->get_MODELO()] = m;
    return true;
}


// search for the model
Model *Stroage::search(const QString model_name)
{
    auto model = this->model_list.find(model_name);
    if( model == this->model_list.end() ){
        return nullptr;
    }
    return model->second;
}

bool Stroage::deduct()
{
    // make sure all items can be found
    QVector<Model*> models;
    models.reserve(30);

    for(Entry* e : EL_deduct.entries){
        QString model_name = e->CLAVE;
        auto model = this->search(model_name);
        if( model == nullptr ){
            return false;
        }
        models.push_back(model);
    }

    for(UI i = 0; i < EL_deduct.size(); i++){
        double CAJA = EL_deduct.get_entry(i)->CAJA;
        if( this->deduct(models[i], CAJA) == false ){
            return false;
        }
    }

    return true;
}

// deduct num from model
bool Stroage::deduct(Model * m, double num) const
{
    if(m == nullptr)
        return false; // model doesn't exist
    double cur_EXIST = m->get_EXISTENCIAS();
    double cur_VENTAS = m->get_VENTAS();
    m->set_EXISTENCIAS(cur_EXIST - num);
    m->set_VENTAS(cur_VENTAS + num);

    this->modify_excel(m);
    return true;
}

// deduct num from the model with corresponding name
bool Stroage::deduct(const QString model_name, double num)
{
    auto model = this->search(model_name);
    if(model == nullptr)
        return false; // model doesn't exist
    double cur_EXIST = model->get_EXISTENCIAS();
    double cur_VENTAS = model->get_VENTAS();
    model->set_EXISTENCIAS(cur_EXIST - num);
    model->set_VENTAS(cur_VENTAS + num);

    this->modify_excel(model);
    return true;
}

bool Stroage::addBack()
{
    // make sure all items can be found
    QVector<Model*> models;
    models.reserve(30);

    for(Entry* e : EL_add.entries){
        QString model_name = e->CLAVE;
        auto model = this->search(model_name);
        if( model == nullptr ){
            return false;
        }
        models.push_back(model);
    }

    for(UI i = 0; i < EL_add.size(); i++){
        double CAJA = EL_add.get_entry(i)->CAJA;
        if( this->addBack(models[i], CAJA) == false ){
            return false;
        }
    }

    return true;
}

bool Stroage::addBack(Model * m, double num) const
{
    if(m == nullptr)
        return false; // model doesn't exist
    double cur_EXIST = m->get_EXISTENCIAS();
    double cur_VENTAS = m->get_VENTAS();
    m->set_EXISTENCIAS(cur_EXIST + num);
    m->set_VENTAS(cur_VENTAS - num);

    this->modify_excel(m);
    return true;
}

bool Stroage::addBack(const QString model_name, double num)
{
    auto model = this->search(model_name);
    if(model == nullptr)
        return false; // model doesn't exist
    double cur_EXIST = model->get_EXISTENCIAS();
    double cur_VENTAS = model->get_VENTAS();
    model->set_EXISTENCIAS(cur_EXIST + num);
    model->set_VENTAS(cur_VENTAS - num);

    this->modify_excel(model);
    return true;
}


void Stroage::modify_excel(Model* model) const
{
    if(excel == nullptr || model == nullptr){ return; }
    excel->set_value(model->get_sheetName(), model->get_row(), model->get_col(),
                     QString::number(model->get_VENTAS()));
    excel->set_value(model->get_sheetName(), model->get_row(), model->get_col(),
                     QString::number(model->get_EXISTENCIAS()));
}
