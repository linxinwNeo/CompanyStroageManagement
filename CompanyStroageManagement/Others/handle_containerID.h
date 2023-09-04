#ifndef HANDLE_CONTAINERID_H
#define HANDLE_CONTAINERID_H

#include <QString>
#include "GlobalVars.h"

inline void handle_ContainerID(QString& ContainerID)
{
    if(ContainerID == none_CN || ContainerID == none_SPAN)
        ContainerID.clear();
}



#endif // HANDLE_CONTAINERID_H
