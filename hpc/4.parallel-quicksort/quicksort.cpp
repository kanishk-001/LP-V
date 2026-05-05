#include <iostream>
#include <vector>
#include <algorithm>
#include <mpi.h>

using namespace std;

// Standard Quicksort for local sorting
void quicksort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[(left + right) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++; j--;
        }
    }
    quicksort(arr, left, j);
    quicksort(arr, i, right);
}

int main(int argc, char** argv) {
    // 1. Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get current process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get total number of processes

    int n;
    // 2. Master process (Rank 0) takes input
    if (rank == 0) {
        cout << "Enter total number of elements (Multiple of " << size << " recommended): " << endl;
        cin >> n;
    }

    // 3. Broadcast size 'n' to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local size for each process
    int local_n = n / size;
    vector<int> data;
    vector<int> local_data(local_n);

    if (rank == 0) {
        data.resize(n);
        cout << "Enter " << n << " elements: " << endl;
        for (int i = 0; i < n; i++) {
            cin >> data[i];
        }
    }

    // 4. Scatter: Distribute chunks of data to all processes
    MPI_Scatter(data.data(), local_n, MPI_INT, local_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // 5. Local Sort: Each process sorts its own chunk
    quicksort(local_data, 0, local_n - 1);

    // 6. Gather: Collect sorted chunks back to Rank 0
    vector<int> sorted_data;
    if (rank == 0) sorted_data.resize(n);

    MPI_Gather(local_data.data(), local_n, MPI_INT, sorted_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // 7. Final Merge (On Rank 0)
    if (rank == 0) {
        // Since chunks are sorted but may overlap, a final pass is needed
        sort(sorted_data.begin(), sorted_data.end());

        cout << "\n--- Final Results ---" << endl;
        cout << "Sorted Array: ";
        for (int i = 0; i < n; i++) {
            cout << sorted_data[i] << " ";
        }
        cout << endl;
    }

    // 8. Clean up
    MPI_Finalize();
    return 0;
}