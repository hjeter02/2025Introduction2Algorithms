#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr ll LIMIT = 1'000'000LL;   // 路徑上限

struct Grid {
    int R, C;                       // 行、列 (地圖範圍)
    int sx, sy, gx, gy;             // 起點、終點 (row, col)
    vector<vector<int>> cell;       // 地圖元素：0=樹木，1=道路，2=毒菇
};

// ---------------------------------------------------------------
// 共用工具
// ---------------------------------------------------------------
const int dx4[4] = {-1, 1, 0, 0};   // 上、下、左、右
const int dy4[4] = {0, 0, -1, 1};
const int dxDiag[4] = {-1, 1, -1, 1};     // 斜向：右上、右下、左上、左下 (依優先順序)
const int dyDiag[4] = {1, 1, -1, -1};

inline bool inside(const Grid &g, int x, int y) { // 確保座標位置在地圖範圍內
    return (0 <= x && x < g.R && 0 <= y && y < g.C);
}

// ---------------------------------------------------------------
// 角色 1：LUKA
// ---------------------------------------------------------------
namespace Luka {
    ll paths;
    Grid *gp;
    vector<vector<char>> vis;   // 訪查標記
    int gx, gy;

    void dfs(int x, int y, int hp, int lastDir) {
        if (paths >= LIMIT) return;           // 若超過上限則停止
        if (x == gx && y == gy) {             // 到達寶藏
            if (hp > 0) ++paths;  //生命值>0 -> 方法數 +1
            return;
        }
        // 生命值不足以到達終點，提前退出
        int dist = abs(gx - x) + abs(gy - y);
        if (hp < dist * 2) return;

        for (int dir = 0; dir < 4; ++dir) {
            if (dir == lastDir) continue;     // 不能連續同方向
            int nx = x + dx4[dir], ny = y + dy4[dir];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue; // 不在地圖範圍內
            int cell = gp->cell[nx][ny];      // 讀取地圖上 下個候選位置的元素
            if (cell == 0) continue;          // 樹木無法穿越
            int cost = 2 + (cell == 2 ? 10 : 0); // 每回合扣生命值
            if (hp <= cost) continue;

            vis[nx][ny] = 1; // 表示這個格子已經被走過
            dfs(nx, ny, hp - cost, dir); // recursive
            vis[nx][ny] = 0;  
        }
    }

    ll solve(const Grid &g) { // 解地圖的函數：初始化 -> DFS -> 回傳結果
        gp = const_cast<Grid *>(&g);
        gx = g.gx; gy = g.gy;
        vis.assign(g.R, vector<char>(g.C, 0)); // 初始化 vis 陣列
        paths = 0;
        vis[g.sx][g.sy] = 1;
        dfs(g.sx, g.sy, 100, -1);
        return min(paths, LIMIT); // 確保如果找到的路徑數量超過了限制，則返回上限值
    }
}

// ---------------------------------------------------------------
// 角色 2：FIZZ
// ---------------------------------------------------------------
namespace Fizz {
    ll paths;
    Grid *gp;
    vector<vector<char>> vis;
    int gx, gy;

    void dfs(int x, int y, int hp) {
        if (paths >= LIMIT) return; // 若超過上限則停止
        if (x == gx && y == gy) {
            if (hp > 0) ++paths;
            return;
        }
        // 距離(斜向移動)
        int dist = max(abs(gx - x), abs(gy - y));
        if (hp < dist) return;               // 每步至少 1 點

        // 依優先序找出可走的斜向格
        vector<pair<int,int>> cand;
        bool hasPoison = false;
        for (int k = 0; k < 4; ++k) {
            int nx = x + dxDiag[k], ny = y + dyDiag[k];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue;
            int cell = gp->cell[nx][ny];
            if (cell == 0) continue;               // 樹木
            cand.emplace_back(nx, ny);
            if (cell == 2) hasPoison = true;
        }
        if (cand.empty()) return;                  // 無路可走

        for (int k = 0; k < 4; ++k) {              // 強制照優先順序嘗試
            int nx = x + dxDiag[k], ny = y + dyDiag[k];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue;
            int cell = gp->cell[nx][ny];
            if (cell == 0) continue;
            if (hasPoison && cell != 2) continue;  // 毒性感知規則

            int cost = 1 + (cell == 2 ? 10 : 0);
            if (hp <= cost) continue;

            vis[nx][ny] = 1;
            dfs(nx, ny, hp - cost); // recursive
            vis[nx][ny] = 0;
        }
    }

    ll solve(const Grid &g) { // 解地圖的函數：初始化 -> DFS -> 回傳結果
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
// 角色 3：JOJO
// ---------------------------------------------------------------
namespace Jojo {
    struct State {
        int x, y, hp, standRemain;  // standRemain: 0=非替身, 1~3 剩餘無敵步數
        bool standAvail;            // 是否尚未啟動替身
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
        // 距離估計 (未計入替身)
        int dist = abs(gx - x) + abs(gy - y);
        if (hp < dist * 3 && standRemain == 0 && !standAvail) return;

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx4[dir], ny = y + dy4[dir];
            if (!inside(*gp, nx, ny) || vis[nx][ny]) continue;
            int cell = gp->cell[nx][ny];
            bool tree = (cell == 0);
            // 兩種可能：
            // A. 不啟用替身
            if (standRemain > 0 || !tree) {
                int newSr = max(0, standRemain - 1);
                int cost = (standRemain > 0) ? 0 : 3 + (cell == 2 ? 10 : 0);
                if (hp > cost) {
                    vis[nx][ny] = 1;
                    dfs(nx, ny, hp - cost, newSr, standAvail);  // recursive
                    vis[nx][ny] = 0;
                }
            }
            // B. 啟用替身 (只要未啟動過且目前沒無敵)
            if (standAvail && standRemain == 0) {
                int newSr = 2;      // 3 回合包含當前步，所以剩餘 2
                int cost = 0;
                vis[nx][ny] = 1;
                dfs(nx, ny, hp - cost, newSr, false);  // recursive
                vis[nx][ny] = 0;
            }
        }
    }

    ll solve(const Grid &g) { // 解地圖的函數：初始化 -> DFS -> 回傳結果
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
// 解析一筆測資並求解
// ---------------------------------------------------------------
ll solveCase(const string &role, const Grid &g) {
    if (role == "luka")      return Luka::solve(g);
    else if (role == "fizz") return Fizz::solve(g);
    else if (role == "jojo") return Jojo::solve(g);
    else return 0;
}

// ---------------------------------------------------------------
int main() {
    // 設置輸出文件
    ofstream fout("110652041_output.txt");
    // 若同目錄存在 input_hw8.txt，優先讀檔；否則走 cin
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
        vector<vector<int>> cell(R, vector<int>(C)); // 地圖
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j)
                *pin >> cell[i][j];
        Grid g{R, C, sx, sy, gx, gy, std::move(cell)};
        answers.push_back(solveCase(role, g)); // 結果存到answer向量當中，最後全部寫入指定輸出檔案
    }
    // 輸出結果 (stdout)
    for (ll v : answers) {
        if (v > LIMIT) v = LIMIT;
        fout << v << "\n";
    }
    return 0;
}