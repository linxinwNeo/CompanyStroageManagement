#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "DataStructures/Container.h"
#include "DataStructures/List.h"
#include "DataStructures/Model.h"
#include "DataStructures/entry.h"
#include <QString>
#include <QVector>

class QuickSorts
{
public:
    QuickSorts() {};
    // sorts vector of models by their MODELCODEs
    static void QuickSort(QVector<ModelPtr>& arr);
    static void QuickSort(QVector<ContainerPtr>& arr);
    static void QuickSort(QVector<EntryPtr>& arr);
    static void QuickSort(QVector<ListPtr>& arr);
};

#endif // QUICKSORT_H
