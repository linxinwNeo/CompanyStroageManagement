#include "model.h"

Model::Model()
{
    this->PZSXCJA = 0.;
    this->INICIAL = 0.;
    this->EXISTENCIAS = 0.;
    this->VENTAS = 0.;
    this->row = -1;
    this->col = -1;
}

Model::Model(const QString MODELO, const QString sheetName, const double PZSXCJA,
             const double INICIAL, const double EXISTENCIAS,
             const double VENTAS, const UI row, const UI col)
{
    this->sheetName = sheetName;
    this->MODELO = MODELO;
    this->PZSXCJA = PZSXCJA;
    this->INICIAL = INICIAL;
    this->EXISTENCIAS = EXISTENCIAS;
    this->VENTAS = VENTAS;
    this->row = row;
    this->col = col;
}

Model::~Model()
{

}

void Model::reset()
{
    this->sheetName = QString();
    this->MODELO = QString();
    this->PZSXCJA = 0.;
    this->INICIAL = 0.;
    this->EXISTENCIAS = 0.;
    this->VENTAS = 0.;
    this->row = -1;
    this->col = -1;
}
