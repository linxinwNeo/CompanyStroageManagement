#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "DataStructures/Container.h"
#include "DataStructures/Model.h"
#include "DataStructures/entry.h"
#include <QString>
#include <QVector>

class QuickSorts
{
public:
    QuickSorts() {};
    // sorts vector of models by their MODELCODEs
    void QuickSort(QVector<ModelPtr>& arr);
    void QuickSort(QVector<ContainerPtr>& arr);
    void QuickSort(QVector<EntryPtr>& arr);

private:
    long int partition(QVector<ModelPtr>& arr, long int start, long int end);
    void QuickSort_(QVector<ModelPtr>& arr, long int start, long int end);

    long int partition(QVector<ContainerPtr>& arr, long int start, long int end);
    void QuickSort_(QVector<ContainerPtr>& arr, long int start, long int end);

    long int partition(QVector<EntryPtr>& arr, long int start, long int end);
    void QuickSort_(QVector<EntryPtr>& arr, long int start, long int end);
};

#endif // QUICKSORT_H
