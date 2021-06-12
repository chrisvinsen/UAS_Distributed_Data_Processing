/*
    How to COMPILE?
        gcc -fopenmp -o quicksort quicksort.c

    How to RUN?
        ./quicksort [size] [processors]
        Parameter [size] is size of array data want to be sorted
        Parameter [processors] is total processor running
        If no parameter are used the program will run with
        default parameters [size] = 16048576 and [processors] = 12
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define DEFAULTSIZE 16048576 // Default size
#define DEFAULTPROCESSOR 4 // Default Processors
#define SEED 918272637

void QuickSort(int arr[], int low, int high) {
    int pivot, i_pivot, temp, left, right;
    if (low >= high) return; // No need to sort

    i_pivot = (low + high) / 2; // Choose the pivot
    pivot = arr[i_pivot];
    left = low;
    right = high;
    while (left <= right) {
        if (arr[left] > pivot) { // Swap left element with right element
            temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            if (right == i_pivot) {
                i_pivot = left;
            }
            right--;
        } else {
            left++;
        }
    }

    // Place the pivot in its place (i.e. swap with right element)
    temp = arr[right];
    arr[right] = pivot;
    arr[i_pivot] = temp;

    // Sort two sublists in parallel using omp task;
    #pragma omp task
        QuickSort(arr, low, (right - 1));
    #pragma omp task
        QuickSort(arr, (right + 1), high);
}

int main(int argc, char *argv[]) {
    int i, size, numProcessors;
    double start_time, end_time;
    long t;

    size = (argc > 1) ? atoi(argv[1]) : DEFAULTSIZE;
    numProcessors= (argc > 2)? atoi(argv[2]) : DEFAULTPROCESSOR;

    omp_set_num_threads(numProcessors); // Set number of processor

    printf("Program running with %d processor and total unordered data = %d\n", numProcessors, size);

    int *Data = (int *)malloc(size * sizeof(int));
    srand(SEED); // Seeding random data

    for(i = 0; i < size; i++) Data[i] = rand(); // Initialize Data

    start_time = omp_get_wtime();
    // Call QuickSort Function, this sorting is done in a parallel region
    #pragma omp parallel
    {
        // We only want to sort the list once, so the first call to QuickSort is done only once
        #pragma omp single
        QuickSort(Data, 0, (size - 1));
    }

    end_time = omp_get_wtime();

    // Check is the data already sorted
    for (i = 0; i < size - 1; i++)
        if (Data[i] > Data[i + 1]) {
            printf("Error... Data masih belum terurut...\n");
            break;
        }

    printf("Waktu eksekusi program (Ts) = %f msec atau %g sec\n", (end_time - start_time) * 1000000, end_time - start_time);

    free(Data);
    return(1);
}