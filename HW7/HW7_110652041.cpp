#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
//int main() {
    // 1. 讀檔名
    string filename;
    if (argc >= 2) {
        filename = argv[1];                   // 從命令列參數取得
    } else {
        cout << "Enter the filename: ";
        if (!(cin >> filename)) {        // 使用者輸入檔名
            cerr << "未取得檔名，程式結束。\n";
            return 1;
        }
    }

    ifstream infile(filename.c_str());
    if (!infile) {
        cerr << "無法開啟檔案: " << filename << '\n';
        return 1;
    }

    // 2. 將多組 8×8 pattern 讀入三維向量
    std::vector<std::vector<std::vector<int>>> patterns;
    std::vector<std::vector<int>> curr;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) {
            if (!curr.empty()) {              // 空行 → 結束一組
                patterns.push_back(curr);
                curr.clear();
            }
            continue;
        }
        std::stringstream ss(line);
        int v;
        std::vector<int> row;
        while (ss >> v) row.push_back(v);
        if (!row.empty()) curr.push_back(row);
    }
    if (!curr.empty()) patterns.push_back(curr); // 最後一組若無空行

    // 3. BFS 計算島嶼
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (size_t idx = 0; idx < patterns.size(); ++idx) {
        const auto& g = patterns[idx];
        int R = g.size();
        int C = g.front().size();
        std::vector<std::vector<char>> vis(R, std::vector<char>(C, 0));
        int islands = 0;

        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j)
                if (g[i][j] == 1 && !vis[i][j]) {
                    ++islands;
                    std::queue<std::pair<int,int>> q;
                    q.push(std::make_pair(i, j));
                    vis[i][j] = 1;
                    while (!q.empty()) {
                        std::pair<int,int> cur = q.front();
                        int x = cur.first, y = cur.second;
                        q.pop();
                        for (int k = 0; k < 4; ++k) {
                            int nx = x + dr[k], ny = y + dc[k];
                            if (nx >= 0 && nx < R && ny >= 0 && ny < C &&
                                g[nx][ny] == 1 && !vis[nx][ny]) {
                                vis[nx][ny] = 1;
                                q.push(std::make_pair(nx, ny));
                            }
                        }
                    }
                }

        // 4. 依指定格式輸出
        std::cout << "Number of Islands(Pattern" << (idx + 1) << "): "
                  << islands << '\n';
    }

    return 0;
}
