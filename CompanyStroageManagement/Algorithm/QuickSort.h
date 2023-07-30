#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "DataStructures/Container.h"
#include "DataStructures/Model.h"
#include <QString>
#include <QVector>

// sorts vector of models by their MODELCODEs
void QuickSort(QVector<ModelPtr>& arr);
long int partition(QVector<ModelPtr>& arr, long int start, long int end);
void QuickSort_(QVector<ModelPtr>& arr, long int start, long int end);


// sorts vector of containers by their IDs
void QuickSort(QVector<ContainerPtr>& arr);
long int partition(QVector<ContainerPtr>& arr, long int start, long int end);
void QuickSort_(QVector<ContainerPtr>& arr, long int start, long int end);

#endif // QUICKSORT_H
