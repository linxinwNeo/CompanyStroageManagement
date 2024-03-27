#include "Algorithm/QuickSort.h"

// sorts vector of models by their MODELCODE
void QuickSorts::QuickSort(QVector<ModelPtr>& arr)
{
    std::sort(arr.begin(), arr.end(), [](const ModelPtr &a, const ModelPtr &b) {
        return a->MODEL_CODE < b->MODEL_CODE;
    });
}



// sorts vector of containers by their ID
void QuickSorts::QuickSort(QVector<ContainerPtr>& arr)
{
    std::sort(arr.begin(), arr.end(), [](const ContainerPtr &a, const ContainerPtr &b) {
        return a->ID < b->ID;
    });
}

// sorts vector of entries by their CLAVE value
void QuickSorts::QuickSort(QVector<EntryPtr>& arr)
{
    std::sort(arr.begin(), arr.end(), [](const EntryPtr &a, const EntryPtr &b) {
        return a->MODEL_CODE < b->MODEL_CODE;
    });
}


// sorts vector of lists by their id
void QuickSorts::QuickSort(QVector<ListPtr>& arr)
{
    std::sort(arr.begin(), arr.end(), [](const ListPtr &a, const ListPtr &b) {
        return a->id < b->id;
    });
}

