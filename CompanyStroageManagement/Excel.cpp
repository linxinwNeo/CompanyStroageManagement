#include "Excel.h"

Excel::Excel(const QString full_path_to_Excel)
{

}

Excel::~Excel()
{
    if(this->axobj != nullptr) delete this->axobj;
    this->axobj = nullptr;
}

void Excel::read_excel()
{

}
