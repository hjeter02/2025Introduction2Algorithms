#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
//int main() {
    string filename;
    if (argc >= 2) {
        filename = argv[1];            
    } else {
        cout << "Enter the filename: ";
        if (!(cin >> filename)) {
            cerr << "fail to read file\n";
            return 1;
        }
    }

    ifstream infile(filename.c_str());
    if (!infile) {
        cerr << "cannot open the file: " << filename << '\n';
        return 1;
    }

    // 2. read 8×8 pattern from file
    std::vector<std::vector<std::vector<int>>> patterns;
    std::vector<std::vector<int>> curr;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) {
            if (!curr.empty()) {              // end of a pattern
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
    if (!curr.empty()) patterns.push_back(curr); // if no trailing newline

    // 3. BFS calculation
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
        std::cout << "Number of Islands(Pattern" << (idx + 1) << "): "
                  << islands << '\n';
    }

    return 0;
}
