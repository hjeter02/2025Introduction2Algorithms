#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <utility>
#include <algorithm>
#include <iomanip> // For setprecision

using namespace std;

// Structure to hold information about the maximum subarray in a given range
struct SubarrayResult {
    double maxSum;  // The maximum sum of the subarray
    int minLen;     // The smallest length of the subarray with max sum
    vector<pair<int, int>> intervals;  // The intervals (start, end) of max subarrays
    // Constructor initializes maxSum to smallest possible
    SubarrayResult() : maxSum(numeric_limits<double>::lowest()), minLen(numeric_limits<int>::max()) {}
};

// Helper function to find the maximum subarray that crosses the midpoint
SubarrayResult findMaxCrossingSubarray(const vector<double>& arr, int low, int mid, int high) {
    SubarrayResult crossRes;
    //For leftSum, scanning leftwards from the midpoint
    double leftSum = numeric_limits<double>::lowest();  // Start with the smallest possible sum
    double sum = 0;  // Temporary sum for calculating left sum
    int maxLeftIndex = mid;
    for (int i = mid; i >= low; --i) {  // Traverse from mid to low (inclusive)
        sum += arr[i];
        if (sum > leftSum) {  // If current sum is greater than the best left sum, update it
            leftSum = sum;
            maxLeftIndex = i;
        }
    }
    //For rightSum, scanning rightwards from the midpoint
    double rightSum = numeric_limits<double>::lowest();  // Start with the smallest possible sum
    sum = 0;  // Temporary sum for calculating right sum
    int maxRightIndex = mid + 1;
    for (int j = mid + 1; j <= high; ++j) {  // Traverse from mid+1 to high (inclusive)
        sum += arr[j];  // Add the current element to sum
        if (sum > rightSum) {  // If current sum is greater than the best right sum, update it
            rightSum = sum;
            maxRightIndex = j;
        }
    }
    //Combine the left and right sums to form the crossing subarray
    crossRes.maxSum = leftSum + rightSum;
    crossRes.intervals.push_back({maxLeftIndex, maxRightIndex});
    
    return crossRes;
}

// Recursive function to find the maximum subarray within arr[low..high]
SubarrayResult findMaxSubarray(const vector<double>& arr, int low, int high) {
    if (low == high) {  // Base case: only one element in the array segment
        SubarrayResult res;
        res.maxSum = arr[low];
        res.minLen = 1;
        res.intervals.push_back({low, low});
        return res;
    }

    int mid = low + (high - low) / 2;  // Define the midpoint of the given array
    //"Divide"
    SubarrayResult leftRes = findMaxSubarray(arr, low, mid);
    SubarrayResult rightRes = findMaxSubarray(arr, mid + 1, high);
    SubarrayResult crossRes = findMaxCrossingSubarray(arr, low, mid, high);
    //Comparing three cases
    double finalMax = leftRes.maxSum;
    if (rightRes.maxSum > finalMax) {  
        finalMax = rightRes.maxSum;
    }
    if (crossRes.maxSum > finalMax) {
        finalMax = crossRes.maxSum;
    }
    // Store the best case
    SubarrayResult result;
    result.maxSum = finalMax;
    // Finding the shortest max. subarray
    result.minLen = numeric_limits<int>::max();  // Initialize the smallest length to a very large value

    // Include candidates from the left half if they tie for max sum
    if (leftRes.maxSum == finalMax) {
        if (leftRes.minLen < result.minLen) {  // If left subarrays are shorter
            result.minLen = leftRes.minLen;
            result.intervals = leftRes.intervals;  // Take the left intervals
        } else if (leftRes.minLen == result.minLen) {  // If they are of the same length
            // If the left half has max-sum subarrays of the same minimal length, add them to the list
            result.intervals.insert(result.intervals.end(), leftRes.intervals.begin(), leftRes.intervals.end());
        }
    }

    // Include candidates from the right half if they tie for max sum
    if (rightRes.maxSum == finalMax) {
        if (rightRes.minLen < result.minLen) {
            result.minLen = rightRes.minLen;
            result.intervals = rightRes.intervals;
        } else if (rightRes.minLen == result.minLen) {
            result.intervals.insert(result.intervals.end(), rightRes.intervals.begin(), rightRes.intervals.end());
        }
    }

    // Include the crossing subarray if it ties for max sum
    if (crossRes.maxSum == finalMax) {
        if (crossRes.minLen < result.minLen) {
            result.minLen = crossRes.minLen;
            result.intervals = crossRes.intervals;
        } else if (crossRes.minLen == result.minLen) {
            result.intervals.insert(result.intervals.end(), crossRes.intervals.begin(), crossRes.intervals.end());
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    string filename;
    if (argc >= 2) {
        filename = argv[1];
    } else {
        cout << "Enter input file path: ";
        if (!(cin >> filename)) {
            cerr << "No input file specified.\n";
            return 1;
        }
    }

    ifstream infile(filename);
    if (!infile) {
        cerr << "Failed to open input file: " << filename << "\n";
        return 1;
    }

    string line;
    vector<double> arr;
    while (getline(infile, line)) {
        stringstream ss(line);
        double index, value;
        ss >> index >> value;  // Read index and value
        arr.push_back(value);   // Only push the value into arr
    }
    infile.close();

    if (arr.empty()) {
        cerr << "No data found in input file.\n";
        return 1;
    }

    SubarrayResult result = findMaxSubarray(arr, 0, arr.size() - 1);

    // Output all subarrays with max sum and the smallest length
    for (auto interval : result.intervals) {
        cout << "Start from " << (interval.first + 1)  // Convert to 1-based index
             << " to " << (interval.second + 1) 
             << endl;
    }
    cout << "Maximum sum is " << fixed << setprecision(2) << result.maxSum << endl;

    return 0;
}
