#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

// ---------------------- Insertion Sort ----------------------
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for(int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while(j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ---------------------- Merge Sort ----------------------
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for(int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for(int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    int i = 0, j = 0, k = left;
    // combine L and R back into arr
    while(i < n1 && j < n2) {
        if(L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    // copy any remaining elements of L (if any)
    while(i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    // copy any remaining elements of R (if any)
    while(j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if(left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// ---------------------- Heap Sort ----------------------
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;  // Initialize largest as root
    int left = 2 * i + 1;   
    int right = 2 * i + 2;  
    // If left child is larger than root
    if(left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    // If right child is larger than largest
    if(right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    // If largest is not root
    if(largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    // Build max heap (rearrange vector)
    for(int i = n/2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    // One by one extract an element from heap
    for(int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);  // Move current root to end
        heapify(arr, i, 0);    // Call max heapify on the reduced heap
    }
}

// ---------------------- Main ----------------------
int main() {
    while(true) {
        cout << "Size: ";
        int size;
        cin >> size;
        if(!cin) return 0;  

        // produce a random array
        vector<int> arr(size);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 100000);
        for(int i = 0; i < size; i++) {
            arr[i] = dist(gen);
        }
        // back up the arr to apply the other sort
        vector<int> arr_backup = arr;

        // -------------- Insertion Sort --------------
        auto start = chrono::high_resolution_clock::now();
        insertionSort(arr);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> insertion_time = end - start;
        cout << "Running time of Insertion Sort: " << insertion_time.count() << " sec\n";

        // -------------- Merge Sort --------------
        arr = arr_backup;  // pick the original unsorted array
        start = chrono::high_resolution_clock::now();
        mergeSort(arr, 0, arr.size() - 1);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> merge_time = end - start;
        cout << "Running time of Merge Sort: " << merge_time.count() << " sec\n";

        // -------------- Heap Sort --------------
        arr = arr_backup;  // pick the original unsorted array
        start = chrono::high_resolution_clock::now();
        heapSort(arr);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> heap_time = end - start;
        cout << "Running time of Heap Sort: " << heap_time.count() << " sec\n";

        cout << "Continue? (Y/N) ";
        char ans;
        cin >> ans;
        if(ans == 'N' || ans == 'n') {
            break;
        }
        cout << "\n";
    }
    cout << "Finish!\n";
    return 0;
}
