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
    // combine
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
    while(i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
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

// ---------------------- Main ----------------------
int main() {
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    while(true) {
        cout << "Size: ";
        int size;
        cin >> size;

        vector<int> arr(size);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 100000);

        for(int i = 0; i < size; i++) {
            arr[i] = dist(gen);
        }

        vector<int> arr_backup = arr;

        // -------------- Insertion Sort --------------
        auto start = chrono::high_resolution_clock::now();
        insertionSort(arr);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> insertion_time = end - start;
        cout << "Running time of Insertion Sort: " << insertion_time.count() << " sec\n";

        // -------------- Merge Sort --------------
        arr = arr_backup;
        start = chrono::high_resolution_clock::now();
        mergeSort(arr, 0, arr.size() - 1);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> merge_time = end - start;
        cout << "Running time of Merge Sort: " << merge_time.count() << " sec\n";

        cout << "Continue？(Y/N) ";
        char ans;
        cin >> ans;
        if(ans == 'N' || ans == 'n') {
            break;
        }
        cout << "\n";
    }
    cout << "Finish！\n";
    return 0;
}
