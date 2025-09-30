#include <bits/stdc++.h>
using namespace std;

// ---------- coefficient ----------
const long long NEG_INF = LLONG_MIN / 4;
const long long POS_INF = LLONG_MAX / 4;

vector<long long> price(11);

// --- Top‑down memory ---
vector<long long> memoMaxPrice, memoMinPrice;
vector<int> memoMaxPieces, memoMinPieces;                
vector<vector<int>> parentMaxTD, parentMinTD;            
// ----------  key ----------
string planKey(const vector<int>& plan) {
    vector<int> s = plan;
    sort(s.begin(), s.end());          
    string k;
    for (int x : s) k += to_string(x) + ",";
    return k;
}

// ---------- Top‑down：max price ----------
void topDownMax(int len) {
    if (memoMaxPrice[len] != NEG_INF) return;  
    if (len == 0) {
        memoMaxPrice[0] = 0;
        memoMaxPieces[0] = 0;
        return;
    }

    long long bestP = NEG_INF;
    int bestPieces = INT_MAX;
    vector<int> bestCuts;

    for (int cut = 1; cut <= min(len, 10); ++cut) {
        topDownMax(len - cut);
        long long candPrice = price[cut] + memoMaxPrice[len - cut];
        int candPieces = 1 + memoMaxPieces[len - cut];
        if (candPrice > bestP || (candPrice == bestP && candPieces < bestPieces)) {
            bestP = candPrice;
            bestPieces = candPieces;
            bestCuts.assign({cut});
        } else if (candPrice == bestP && candPieces == bestPieces) {
            bestCuts.push_back(cut);
        }
    }
    memoMaxPrice[len] = bestP;
    memoMaxPieces[len] = bestPieces;
    parentMaxTD[len] = bestCuts;
}

// ---------- Top‑down：min. price ----------
void topDownMin(int len) {
    if (memoMinPrice[len] != POS_INF) return;
    if (len == 0) {
        memoMinPrice[0] = 0;
        memoMinPieces[0] = 0;
        return;
    }

    long long bestP = POS_INF;
    int bestPieces = INT_MAX;
    vector<int> bestCuts;

    for (int cut = 1; cut <= min(len, 10); ++cut) {
        topDownMin(len - cut);
        long long candPrice = price[cut] + memoMinPrice[len - cut];
        int candPieces = 1 + memoMinPieces[len - cut];
        if (candPrice < bestP || (candPrice == bestP && candPieces < bestPieces)) {
            bestP = candPrice;
            bestPieces = candPieces;
            bestCuts.assign({cut});
        } else if (candPrice == bestP && candPieces == bestPieces) {
            bestCuts.push_back(cut);
        }
    }
    memoMinPrice[len] = bestP;
    memoMinPieces[len] = bestPieces;
    parentMinTD[len] = bestCuts;
}

// ---------- DFS ----------
void dfsPlans(int len,
              const vector<vector<int>>& parent,
              vector<int>& cur,
              vector<vector<int>>& out,
              unordered_set<string>& seen) {
    if (len == 0) {
        string k = planKey(cur);
        if (seen.insert(k).second) {
            vector<int> s = cur;
            sort(s.begin(), s.end()); 
            out.push_back(s);
        }
        return;
    }
    for (int cut : parent[len]) {
        cur.push_back(cut);
        dfsPlans(len - cut, parent, cur, out, seen);
        cur.pop_back();
    }
}

// ---------- print ----------
void printPlanSpace(const vector<int>& plan) {
    for (size_t i = 0; i < plan.size(); ++i) {
        if (i) cout << ' ';
        cout << plan[i];
    }
}

// ---------- Bottom‑up DP ----------
struct BUResult {
    vector<long long> price;           // dp price
    vector<int> pieces;               // minimal pieces
    vector<vector<int>> parent;       // first‑cut list
};

BUResult bottomUp(int N) {
    BUResult R;
    R.price.assign(N + 1, NEG_INF);
    R.pieces.assign(N + 1, INT_MAX);
    R.parent.assign(N + 1, {});
    R.price[0] = 0;
    R.pieces[0] = 0;

    for (int len = 1; len <= N; ++len) {
        long long bestMax = NEG_INF;
        int bestPieces = INT_MAX;
        vector<int> cuts;
        for (int cut = 1; cut <= min(len, 10); ++cut) {
            long long candP = price[cut] + R.price[len - cut];
            int candPieces = 1 + R.pieces[len - cut];
            if (candP > bestMax || (candP == bestMax && candPieces < bestPieces)) {
                bestMax = candP;
                bestPieces = candPieces;
                cuts.assign({cut});
            } else if (candP == bestMax && candPieces == bestPieces) {
                cuts.push_back(cut);
            }
        }
        R.price[len] = bestMax;
        R.pieces[len] = bestPieces;
        R.parent[len] = cuts;
    }
    return R;
}

// ---------- Bottom‑up DP (MIN) ----------
BUResult bottomUpMin(int N) {
    BUResult R;
    R.price.assign(N + 1, POS_INF);
    R.pieces.assign(N + 1, INT_MAX);
    R.parent.assign(N + 1, {});
    R.price[0] = 0;
    R.pieces[0] = 0;

    for (int len = 1; len <= N; ++len) {
        long long bestMin = POS_INF;
        int bestPieces = INT_MAX;
        vector<int> cuts;
        for (int cut = 1; cut <= min(len, 10); ++cut) {
            long long candP = price[cut] + R.price[len - cut];
            int candPieces = 1 + R.pieces[len - cut];
            if (candP < bestMin || (candP == bestMin && candPieces < bestPieces)) {
                bestMin = candP;
                bestPieces = candPieces;
                cuts.assign({cut});
            } else if (candP == bestMin && candPieces == bestPieces) {
                cuts.push_back(cut);
            }
        }
        R.price[len] = bestMin;
        R.pieces[len] = bestPieces;
        R.parent[len] = cuts;
    }
    return R;
}

int main() {
    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);

    // ===== input =====
    int N;
    cout << "input rod of length: ";
    if (!(cin >> N) || N <= 0) {
        cerr << "input error: please input positive number N\n";
        return 1;
    }
    cout << "input price of rod of length 1~10: ";
    for (int i = 1; i <= 10; ++i) {
        if (!(cin >> price[i]) || price[i] < 0) {
            cerr << "input error: please input 10 nonnegative price in order\n";
            return 1;
        }
    }

    // ===== Top‑down =====
    memoMaxPrice.assign(N + 1, NEG_INF);
    memoMinPrice.assign(N + 1, POS_INF);
    memoMaxPieces.assign(N + 1, INT_MAX);
    memoMinPieces.assign(N + 1, INT_MAX);
    parentMaxTD.assign(N + 1, {});
    parentMinTD.assign(N + 1, {});

    topDownMax(N);
    topDownMin(N);

    // (Top‑down)
    vector<int> cur;
    vector<vector<int>> allMaxTD, allMinTD;
    unordered_set<string> seenMaxTD, seenMinTD;
    dfsPlans(N, parentMaxTD, cur, allMaxTD, seenMaxTD);
    dfsPlans(N, parentMinTD, cur, allMinTD, seenMinTD);

    auto cmp = [](const vector<int>& a, const vector<int>& b) {
        if (a.size() != b.size()) return a.size() < b.size();
        return a < b;
    };
    sort(allMaxTD.begin(), allMaxTD.end(), cmp);
    sort(allMinTD.begin(), allMinTD.end(), cmp);

    // ===== Bottom‑up =====
    BUResult buMax = bottomUp(N);
    BUResult buMin = bottomUpMin(N);

    vector<vector<int>> allMaxBU, allMinBU;
    unordered_set<string> seenMaxBU, seenMinBU;
    dfsPlans(N, buMax.parent, cur, allMaxBU, seenMaxBU);
    dfsPlans(N, buMin.parent, cur, allMinBU, seenMinBU);
    sort(allMaxBU.begin(), allMaxBU.end(), cmp);
    sort(allMinBU.begin(), allMinBU.end(), cmp);

    // ===== output =====
    cout << "\nTop-down:\n";
    cout << "maximum price: " << memoMaxPrice[N] << '\n';
    for (const auto& p : allMaxTD) {
        printPlanSpace(p);
        cout << '\n';
    }
    cout << "number of pieces: " << memoMaxPieces[N] << "\n\n";

    cout << "minimum price: " << memoMinPrice[N] << '\n';
    for (const auto& p : allMinTD) {
        printPlanSpace(p);
        cout << '\n';
    }
    cout << "number of pieces: " << memoMinPieces[N] << "\n\n";

    cout << "Bottom-up:\n";
    cout << "maximum price: " << buMax.price[N] << '\n';
    for (const auto& p : allMaxBU) {
        printPlanSpace(p);
        cout << '\n';
    }
    cout << "number of pieces: " << buMax.pieces[N] << "\n\n";

    cout << "minimum price: " << buMin.price[N] << '\n';
    for (const auto& p : allMinBU) {
        printPlanSpace(p);
        cout << '\n';
    }
    cout << "number of pieces: " << buMin.pieces[N] << '\n';

    return 0;
}
