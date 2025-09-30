 #include <bits/stdc++.h>
 using namespace std;
 
 vector<double> read_sequence() {
     string line;
     getline(cin, line);
     istringstream iss(line);
     vector<double> seq;
     double x;
     while (iss >> x) seq.push_back(x);
     return seq;
 }
 
 int main() {
     cout << "Please enter sequences with elements separated by spaces. \n";
     cout << "Enter the elements of sequence X: ";
     vector<double> X = read_sequence();
     cout << "Enter the elements of sequence Y: ";
     vector<double> Y = read_sequence();
     cout << "Enter the elements of sequence Z: ";
     vector<double> Z = read_sequence();
 
     int n1 = (int)X.size(), n2 = (int)Y.size(), n3 = (int)Z.size();

     vector<vector<vector<int>>> dp(
         n1 + 1, vector<vector<int>>(n2 + 1, vector<int>(n3 + 1, 0)));
 
     for (int i = 1; i <= n1; ++i)
         for (int j = 1; j <= n2; ++j)
             for (int k = 1; k <= n3; ++k)
                 if (X[i - 1] == Y[j - 1] && Y[j - 1] == Z[k - 1])
                     dp[i][j][k] = dp[i - 1][j - 1][k - 1] + 1;
                 else
                     dp[i][j][k] = max({dp[i - 1][j][k],
                                        dp[i][j - 1][k],
                                        dp[i][j][k - 1]});
 
     int L = dp[n1][n2][n3];           // LCS length
     vector<double> W;  W.reserve(L);  // LCS
 
     // ---------------------- LCS -----------------------
     int i = n1, j = n2, k = n3;
     while (i && j && k) {
         if (X[i - 1] == Y[j - 1] && Y[j - 1] == Z[k - 1]) {
             W.push_back(X[i - 1]);
             --i; --j; --k;
         } else if (dp[i - 1][j][k] >= dp[i][j - 1][k] &&
                    dp[i - 1][j][k] >= dp[i][j][k - 1]) {
             --i;
         } else if (dp[i][j - 1][k] >= dp[i][j][k - 1]) {
             --j;
         } else {
             --k;
         }
     }
     reverse(W.begin(), W.end());
 
     // -------------------------- output ---------------------------
     //cout << "\n===== Result =====\n";
     cout << "Longest Common Subsequence (LCS): ";
     for (size_t t = 0; t < W.size(); ++t)
         cout << W[t] << " ";
     cout << "\n";
     cout << "Length of LCS: " << L << '\n';
     return 0;
 }
 