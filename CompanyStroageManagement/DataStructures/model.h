#ifndef MODEL_H
#define MODEL_H

#include "Predefined.h"
#include "xlsxcelllocation.h"
#include <QString>

struct Container{
    QString type; // CONTENEDOR or PEDIMENTO or CUBREBOCAS
    QString ID; // usually numbers with spaces
    QString num;
};

class Model
{
private:
    QString sheetName;
    // cell location of the MODELO
    UI row;
    UI col;

    Container container; // can be empty...

    QString MODELO; // identifier of this model

    double PZSXCJA; // sometimes called PIEZAS
    double INICIAL;
    double EXISTENCIAS;
    double VENTAS;

public:
    Model();
    Model(const QString, const QString, const double, const double,
          const double, const double, const UI, const UI);
    ~Model();

    void set_MODELO(const QString MODELO) {this->MODELO = MODELO;};
    void set_PZSXCJA(const double PZSXCJA) {this->PZSXCJA = PZSXCJA;};
    void set_INICIAL(const double INICIAL){this->INICIAL = INICIAL;};
    void set_EXISTENCIAS(const double EXISTENCIAS){this->EXISTENCIAS = EXISTENCIAS;};
    void set_VENTAS(const double VENTAS){this->VENTAS = VENTAS;};
    void set_sheetName(const QString sheetName){this->sheetName = sheetName;};
    void set_row_col(const UI row, const UI col){
        this->row = row;
        this->col = col;
    };

    QString get_MODELO() {return MODELO;};
    double get_PZSXCJA() {return PZSXCJA;};
    double get_INICIAL() {return INICIAL;};
    double get_EXISTENCIAS() {return EXISTENCIAS;};
    double get_VENTAS() {return VENTAS;};
    QString get_sheetName(){return this->sheetName;};
    UI get_row(){return this->row;};
    UI get_col(){return this->col;};

    void reset();
};

#endif // MODEL_H
