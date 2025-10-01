#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr ll LIMIT = 1'000'000LL;   // Path limit

struct Grid {
    int R, C;                       // Rows, Columns (map size)
    int sx, sy, gx, gy;             // Start point, Goal point (row, col)
    vector<vector<int>> cell;       // Map elements: 0 = tree, 1 = road, 2 = poison mushroom
};

// ---------------------------------------------------------------
// Common utilities
// ---------------------------------------------------------------
const int dx4[4] = {-1, 1, 0, 0};   // Up, Down, Left, Right
const int dy4[4] = {0, 0, -1, 1};
const int dxDiag[4] = {-1, 1, -1, 1};     // Diagonal: top-right, bottom-right, top-left, bottom-left (priority order)
const int dyDiag[4] = {1, 1, -1, -1};

inline bool inside(const Grid &g, int x, int y) { // Ensure coordinate is inside the map
    return (0 <= x && x < g.R && 0 <= y && y < g.C);
}

// ---------------------------------------------------------------
// LUKA
// ---------------------------------------------------------------
namespace Luka {
    ll paths;
    Grid *gp;
    vector<vector<char>> vis;   // Visited markers
    int gx, gy;

    void dfs(int x, int y, int hp, int lastDir) {
        if (paths >= LIMIT) return;           // Stop if exceeds limit
        if (x == gx && y == gy) {             // Reached treasure
            if (hp > 0) ++paths;  // HP > 0 -> count as one path
            return;
        }
        // If HP not enough to reach goal, exit early
        int dist = abs(gx - x) + abs(gy - y);
        if (hp < dist * 2) return;

        for (int dir = 0; dir < 4; ++dir) {
            if (dir == lastDir) continue;     // Cannot move in same direction consecutively
            int nx = x + dx4[dir], ny = y + dy4[dir];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue; // Outside map or already visited
            int cell = gp->cell[nx][ny];      // Next cell content
            if (cell == 0) continue;          // Cannot pass through trees
            int cost = 2 + (cell == 2 ? 10 : 0); // HP cost per step
            if (hp <= cost) continue;

            vis[nx][ny] = 1; // Mark as visited
            dfs(nx, ny, hp - cost, dir); // recursive
            vis[nx][ny] = 0;  
        }
    }

    ll solve(const Grid &g) { // Solve map: initialize -> DFS -> return result
        gp = const_cast<Grid *>(&g);
        gx = g.gx; gy = g.gy;
        vis.assign(g.R, vector<char>(g.C, 0)); // Initialize visited array
        paths = 0;
        vis[g.sx][g.sy] = 1;
        dfs(g.sx, g.sy, 100, -1);
        return min(paths, LIMIT); // Ensure return does not exceed limit
    }
}

// ---------------------------------------------------------------
// FIZZ
// ---------------------------------------------------------------
namespace Fizz {
    ll paths;
    Grid *gp;
    vector<vector<char>> vis;
    int gx, gy;

    void dfs(int x, int y, int hp) {
        if (paths >= LIMIT) return; // Stop if exceeds limit
        if (x == gx && y == gy) {
            if (hp > 0) ++paths;
            return;
        }
        // Distance (diagonal movement)
        int dist = max(abs(gx - x), abs(gy - y));
        if (hp < dist) return;               // At least 1 HP per step

        // Collect diagonal candidate moves
        vector<pair<int,int>> cand;
        bool hasPoison = false;
        for (int k = 0; k < 4; ++k) {
            int nx = x + dxDiag[k], ny = y + dyDiag[k];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue;
            int cell = gp->cell[nx][ny];
            if (cell == 0) continue;               // tree
            cand.emplace_back(nx, ny);
            if (cell == 2) hasPoison = true;
        }
        if (cand.empty()) return;                  // No available path

        for (int k = 0; k < 4; ++k) {              // Must follow priority order
            int nx = x + dxDiag[k], ny = y + dyDiag[k];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue;
            int cell = gp->cell[nx][ny];
            if (cell == 0) continue;
            if (hasPoison && cell != 2) continue;  // Poison-sensing rule

            int cost = 1 + (cell == 2 ? 10 : 0);
            if (hp <= cost) continue;

            vis[nx][ny] = 1;
            dfs(nx, ny, hp - cost); // recursive
            vis[nx][ny] = 0;
        }
    }

    ll solve(const Grid &g) { // Solve map: initialize -> DFS -> return result
        gp = const_cast<Grid *>(&g);
        gx = g.gx; gy = g.gy;
        vis.assign(g.R, vector<char>(g.C, 0));
        paths = 0;
        vis[g.sx][g.sy] = 1;
        dfs(g.sx, g.sy, 50);
        return min(paths, LIMIT);
    }
}

// ---------------------------------------------------------------
// JOJO
// ---------------------------------------------------------------
namespace Jojo {
    struct State {
        int x, y, hp, standRemain;  // standRemain: 0=not active, 1~3 remaining invincible steps
        bool standAvail;            // Whether Stand can still be activated
    };

    ll paths;
    Grid *gp;
    vector<vector<char>> vis;
    int gx, gy;

    void dfs(int x, int y, int hp, int standRemain, bool standAvail) {
        if (paths >= LIMIT) return;
        if (x == gx && y == gy) {
            if (hp > 0) ++paths;
            return;
        }
        // Distance estimation (ignores Stand)
        int dist = abs(gx - x) + abs(gy - y);
        if (hp < dist * 3 && standRemain == 0 && !standAvail) return;

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx4[dir], ny = y + dy4[dir];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue;
            int cell = gp->cell[nx][ny];
            bool tree = (cell == 0);
            // Two possibilities:
            // A. Without Stand
            if (standRemain > 0 || !tree) {
                int newSr = max(0, standRemain - 1);
                int cost = (standRemain > 0) ? 0 : 3 + (cell == 2 ? 10 : 0);
                if (hp > cost) {
                    vis[nx][ny] = 1;
                    dfs(nx, ny, hp - cost, newSr, standAvail);  // recursive
                    vis[nx][ny] = 0;
                }
            }
            // B. Activate Stand (only if not used yet and not currently active)
            if (standAvail && standRemain == 0) {
                int newSr = 2;      // 3 turns including current -> remaining 2
                int cost = 0;
                vis[nx][ny] = 1;
                dfs(nx, ny, hp - cost, newSr, false);  // recursive
                vis[nx][ny] = 0;
            }
        }
    }

    ll solve(const Grid &g) { // Solve map: initialize -> DFS -> return result
        gp = const_cast<Grid *>(&g);
        gx = g.gx; gy = g.gy;
        vis.assign(g.R, vector<char>(g.C, 0));
        paths = 0;
        vis[g.sx][g.sy] = 1;
        dfs(g.sx, g.sy, 100, 0, true);
        return min(paths, LIMIT);
    }
}

// ---------------------------------------------------------------
// Parse a test case and solve
// ---------------------------------------------------------------
ll solveCase(const string &role, const Grid &g) {
    if (role == "luka")      return Luka::solve(g);
    else if (role == "fizz") return Fizz::solve(g);
    else if (role == "jojo") return Jojo::solve(g);
    else return 0;
}

// ---------------------------------------------------------------
int main() {
    // Set output file
    ofstream fout("110652041_output.txt");
    // If input_hw8.txt exists in the same directory, read it; otherwise, use cin
    ifstream fin("input_hw8.txt");
    istream *pin = &cin;
    if (fin.good()) pin = &fin;

    string role;
    vector<ll> answers;

    while (*pin >> role) {
        if (role.empty()) continue;
        int R, C; *pin >> R >> C;
        int sx, sy; *pin >> sx >> sy;
        int gx, gy; *pin >> gx >> gy;
        vector<vector<int>> cell(R, vector<int>(C)); // map
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j)
                *pin >> cell[i][j];
        Grid g{R, C, sx, sy, gx, gy, std::move(cell)};
        answers.push_back(solveCase(role, g)); // Store results into answers vector
    }
    // Output results (stdout)
    for (ll v : answers) {
        if (v > LIMIT) v = LIMIT;
        fout << v << "\n";
    }
    return 0;
}