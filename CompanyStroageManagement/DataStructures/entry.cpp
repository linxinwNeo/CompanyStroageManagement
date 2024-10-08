#include "DataStructures/entry.h"
#include "GlobalVars.h"
#include <QLocale>

Entry::Entry(unsigned long NUM_PIECES, unsigned long NUM_PIECES_PER_BOX,
             QString MODEL_CODE, QString ContainerID,
             QString Description_SPAN, QString Description_CN,
             double PRIZE_PER_PIECE)
{
    this->MODEL_CODE = MODEL_CODE;
    this->NUM_PIECES = NUM_PIECES;
    this->NUM_PIECES_PER_BOX = NUM_PIECES_PER_BOX;
    this->Description_SPAN = Description_SPAN;
    this->Description_CN = Description_CN;
    this->PRICE_PER_PIECE = PRIZE_PER_PIECE;
    this->TOTAL = ((double)NUM_PIECES) * PRIZE_PER_PIECE; // total is computed here
    this->ContainerID = ContainerID;
}


// search inventory, and return the model with matching info
ModelPtr Entry::get_corresponding_model() const
{
    const QString& MODELCODE = this->MODEL_CODE;
    const QString& CONTAINER_ID = this->ContainerID;
    ModelPtr model = inventory.get_Model(MODELCODE, CONTAINER_ID);

    return model;
}


QVector<QString> Entry::view_values() const
{
    return {
        this->MODEL_CODE,
        this->ContainerID,

        Description_CN,
        Description_SPAN,

        GlobalVars::locale.toString(this->num_boxes(), 'f', 2),
        GlobalVars::locale.toString(this->NUM_PIECES_PER_BOX),
        GlobalVars::locale.toString(NUM_PIECES),
        GlobalVars::locale.toString(this->PRICE_PER_PIECE, 'f', 2),
        GlobalVars::locale.toString(this->TOTAL, 'f', 2),
    };
}


// 计算箱数
double Entry::num_boxes() const
{
    const double number_of_pieces = this->NUM_PIECES;
    const double number_of_pieces_per_box = this->NUM_PIECES_PER_BOX;
    const double num = number_of_pieces / number_of_pieces_per_box;
    return num;
}
