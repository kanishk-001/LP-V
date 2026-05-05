#include <iostream>
#include <vector>
#include <omp.h>
#include <climits> // For INT_MIN and INT_MAX

using namespace std;

void performReduction(vector<int>& arr) {
    int n = arr.size();
    
    // Initialize variables
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    long long sum = 0;

    // Parallel Reduction
    // The 'reduction' clause handles the synchronization (locks) for us automatically.
    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        sum += arr[i];
    }

    // Average is calculated from the reduced sum
    double average = (double)sum / n;

    cout << "\n--- Reduction Results ---" << endl;
    cout << "Minimum: " << min_val << endl;
    cout << "Maximum: " << max_val << endl;
    cout << "Sum:     " << sum << endl;
    cout << "Average: " << average << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid size!" << endl;
        return 1;
    }

    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    performReduction(arr);

    return 0;
}