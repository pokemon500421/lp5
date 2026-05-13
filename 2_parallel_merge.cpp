#include <iostream>
#include <omp.h>

using namespace std;

// Function to print array
void printArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Merge function
void merge(int arr[], int low, int mid, int high) {

    int n1 = mid - low + 1;
    int n2 = high - mid;

    int left[n1];
    int right[n2];

    // Copy left partition
    for (int i = 0; i < n1; i++)
        left[i] = arr[low + i];

    // Copy right partition
    for (int j = 0; j < n2; j++)
        right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;

    // Merge arrays
    while (i < n1 && j < n2) {

        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }

        k++;
    }

    // Remaining left elements
    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    // Remaining right elements
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

// Parallel Merge Sort
void parallelMergeSort(int arr[], int low, int high) {

    if (low < high) {

        int mid = (low + high) / 2;

        #pragma omp parallel sections
        {

            #pragma omp section
            {
                parallelMergeSort(arr, low, mid);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, high);
            }
        }

        merge(arr, low, mid, high);
    }
}

// Sequential Merge Sort
void mergeSort(int arr[], int low, int high) {

    if (low < high) {

        int mid = (low + high) / 2;

        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);

        merge(arr, low, mid, high);
    }
}

int main() {

    int n = 10;

    int arr1[n];
    int arr2[n];

    double start_time, end_time;

    // Initialize arrays
    for(int i = 0, j = n; i < n; i++, j--) {
        arr1[i] = j;
        arr2[i] = j;
    }

    // Original Array
    cout << "Original Array: ";
    printArray(arr1, n);

    // Sequential Merge Sort
    start_time = omp_get_wtime();

    mergeSort(arr1, 0, n - 1);

    end_time = omp_get_wtime();

    cout << "\nSorted Array using Sequential Merge Sort: ";
    printArray(arr1, n);

    cout << "Time taken by Sequential Merge Sort: "
         << end_time - start_time
         << " seconds\n";

    // Parallel Merge Sort
    start_time = omp_get_wtime();

    parallelMergeSort(arr2, 0, n - 1);

    end_time = omp_get_wtime();

    cout << "\nSorted Array using Parallel Merge Sort: ";
    printArray(arr2, n);

    cout << "Time taken by Parallel Merge Sort: "
         << end_time - start_time
         << " seconds\n";

    return 0;
}
