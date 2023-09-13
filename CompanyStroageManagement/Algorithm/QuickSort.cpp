#include "Algorithm/QuickSort.h"

long int QuickSorts::partition(QVector<ModelPtr>& arr, long int start, long int end){
    ModelPtr pivot_model = arr[start];

    long int count = 0;
    for (long int i = start + 1; i <= end; i++) {
        if (arr[i]->MODEL_CODE <= pivot_model->MODEL_CODE)
            count++;
    }

    // Giving pivot element its correct position
    long int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    // Sorting left and right parts of the pivot element
    long int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i]->MODEL_CODE <= pivot_model->MODEL_CODE) {
            i++;
        }

        while (arr[j]->MODEL_CODE > pivot_model->MODEL_CODE) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}


void QuickSorts::QuickSort_(QVector<ModelPtr>& arr, long int start, long int end)
{
    // base case
    if (start >= end)
        return;

    // partitioning the array
    long int p = partition(arr, start, end);

    // Sorting the left part
    QuickSort_(arr, start, p - 1);

    // Sorting the right part
    QuickSort_(arr, p + 1, end);
}


// sorts vector of models by their MODELCODE
void QuickSorts::QuickSort(QVector<ModelPtr>& arr)
{
    QuickSort_(arr, 0, arr.size() - 1);
}



// sorts vector of containers by their ID
void QuickSorts::QuickSort(QVector<ContainerPtr>& arr)
{
    QuickSort_(arr, 0, arr.size() - 1);
}


long int QuickSorts::partition(QVector<ContainerPtr>& arr, long int start, long int end){
    ContainerPtr pivot_model = arr[start];

    long int count = 0;
    for (long int i = start + 1; i <= end; i++) {
        if (arr[i]->ID <= pivot_model->ID)
            count++;
    }

    // Giving pivot element its correct position
    long int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    // Sorting left and right parts of the pivot element
    long int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i]->ID <= pivot_model->ID) {
            i++;
        }

        while (arr[j]->ID > pivot_model->ID) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}


void QuickSorts::QuickSort_(QVector<ContainerPtr>& arr, long int start, long int end)
{
    // base case
    if (start >= end)
        return;

    // partitioning the array
    long int p = partition(arr, start, end);

    // Sorting the left part
    QuickSort_(arr, start, p - 1);

    // Sorting the right part
    QuickSort_(arr, p + 1, end);
}



// sorts vector of entries by their MODELCODE/CLAVE
void QuickSorts::QuickSort(QVector<EntryPtr>& arr)
{
    QuickSort_(arr, 0, arr.size() - 1);
}


long int QuickSorts::partition(QVector<EntryPtr>& arr, long int start, long int end){
    EntryPtr pivot_entry = arr[start];

    long int count = 0;
    for (long int i = start + 1; i <= end; i++) {
        if (arr[i]->CLAVE <= pivot_entry->CLAVE)
            count++;
    }

    // Giving pivot element its correct position
    long int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    // Sorting left and right parts of the pivot element
    long int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i]->CLAVE <= pivot_entry->CLAVE) {
            i++;
        }

        while (arr[j]->CLAVE > pivot_entry->CLAVE) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}


void QuickSorts::QuickSort_(QVector<EntryPtr>& arr, long int start, long int end)
{
    // base case
    if (start >= end)
        return;

    // partitioning the array
    long int p = partition(arr, start, end);

    // Sorting the left part
    QuickSort_(arr, start, p - 1);

    // Sorting the right part
    QuickSort_(arr, p + 1, end);
}

