#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// --- Bubble Sort Logic ---
void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
    }
}

void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
    }
}

// --- Merge Sort Logic ---
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> left(n1), right(n2);
    for (int i = 0; i < n1; i++) left[i] = arr[l + i];
    for (int i = 0; i < n2; i++) right[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
    }
    while (i < n1) arr[k++] = left[i++];
    while (j < n2) arr[k++] = right[j++];
}

void mergeSortSeq(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortSeq(arr, l, m);
        mergeSortSeq(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void mergeSortParallel(vector<int>& arr, int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;
        if (depth <= 0) {
            mergeSortSeq(arr, l, m);
            mergeSortSeq(arr, m + 1, r);
        } else {
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSortParallel(arr, l, m, depth - 1);
                #pragma omp section
                mergeSortParallel(arr, m + 1, r, depth - 1);
            }
        }
        merge(arr, l, m, r);
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> data(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) cin >> data[i];

    double start, end;
    vector<int> temp;

    // Sequential Bubble
    temp = data;
    start = omp_get_wtime();
    bubbleSortSeq(temp);
    end = omp_get_wtime();
    cout << "Seq Bubble Sort Time: " << (end - start) << "s" << endl;

    // Parallel Bubble
    temp = data;
    start = omp_get_wtime();
    bubbleSortParallel(temp);
    end = omp_get_wtime();
    cout << "Par Bubble Sort Time: " << (end - start) << "s" << endl;

    // Sequential Merge
    temp = data;
    start = omp_get_wtime();
    mergeSortSeq(temp, 0, n - 1);
    end = omp_get_wtime();
    cout << "Seq Merge Sort Time:  " << (end - start) << "s" << endl;

    // Parallel Merge
    temp = data;
    start = omp_get_wtime();
    mergeSortParallel(temp, 0, n - 1, 2);
    end = omp_get_wtime();
    cout << "Par Merge Sort Time:  " << (end - start) << "s" << endl;

    cout << "\nSorted Array: ";
    for (int i = 0; i < n; i++) cout << temp[i] << " ";
    cout << endl;

    return 0;
}