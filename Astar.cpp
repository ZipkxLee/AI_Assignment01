#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <filesystem>
#include <chrono>

struct Node {
    vector<int> solution;
    int g;  // cost so far（目前已滿足的子句數）
    int h;  // estimated cost to goal（還需要滿足的子句數）

    int f() const { return g + h; }

    // 小根堆（f 最小的先）
    bool operator<(const Node& other) const {
        return f() > other.f();
    }
};

// 自動從檔名擷取 Dim 值
int catchDimFromFilename(const string& filename) {
    size_t pos = filename.find("Dim=");
    if (pos == string::npos) return -1;
    pos += 4;
    size_t end = filename.find_first_not_of("0123456789", pos);
    string dimStr = filename.substr(pos, end - pos);
    return stoi(dimStr);
}

// 檢查單一子句是否被滿足
bool checkClause(const vector<int>& sol, int a, int b, int c) {
    totalClauseChecks++;  // 🔸 每次比對都累加
    auto getVal = [&](int var) -> int {
        int idx = abs(var) - 1;
        if (idx >= sol.size()) return 0; // 未指派，當成 false
        int val = sol[idx];
        return (var > 0) ? val : -val;
    };
    return (getVal(a) == 1 || getVal(b) == 1 || getVal(c) == 1);
}

// Early pruning：檢查是否已造成某子句無法滿足
bool isValidPartialAssignment(const vector<int>& sol) {
    for (int i = 0; i < clauseCount; i++) {
        bool clauseSat = false, hasUnassigned = false;
        for (int j = 0; j < 3; j++) {
            int lit = clauses[i][j];
            int idx = abs(lit) - 1;
            if (idx >= sol.size()) {
                hasUnassigned = true;
                continue;
            }
            int val = sol[idx];
            if ((lit > 0 && val == 1) || (lit < 0 && val == -1)) {
                clauseSat = true;
                break;
            }
        }
        if (!clauseSat && !hasUnassigned) return false;
    }
    return true;
}

// 回傳目前已滿足的子句數
int satisfiedClauses(const vector<int>& sol) {
    int count = 0;
    for (int i = 0; i < clauseCount; i++)
        if (checkClause(sol, clauses[i][0], clauses[i][1], clauses[i][2]))
            count++;
    return count;
}

// 回傳還需要滿足多少子句
int heuristic(const vector<int>& sol) {
    vector<int> tmp = sol;
    tmp.resize(D, 0);  // 預設未指定為 false
    return clauseCount - satisfiedClauses(tmp);
}

// 編碼當前狀態（避免重複展開）
string encodeState(const vector<int>& sol) {
    string code;
    for (int v : sol) code += (v == 1 ? '1' : '0');
    return code;
}

int main() {
    auto start = high_resolution_clock::now();
    string fileName;
    string defaultPath = "./data/3SAT_Dim=10.csv";
    cout << "CSV file path ( press enter for the default path: \"" << defaultPath << "\"): ";
    getline(cin, fileName);
    if (fileName.empty()) fileName = defaultPath;

    D = catchDimFromFilename(fileName);
    if (D <= 0) {
        cout << "Can't catch Dim from filename!" << endl;
        return 1;
    }

    ifstream fin(fileName);
    if (!fin) {
        cout << "Error opening file: " << fileName << endl;
        return 1;
    }

    int a, b, c;
    char comma;
    while (fin >> a >> comma >> b >> comma >> c) {
        clauses[clauseCount][0] = a;
        clauses[clauseCount][1] = b;
        clauses[clauseCount][2] = c;
        clauseCount++;
    }
    fin.close();

    // 建立輸出資料夾
    fs::path outDir = "results/Astar";
    if (!fs::exists(outDir)) fs::create_directories(outDir);

    priority_queue<Node> openList;
    unordered_set<string> visited;

    Node startNode{{}, 0, heuristic({})};
    openList.push(startNode);
    visited.insert(encodeState(startNode.solution));

    Node bestNode;
    bool found = false;

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();
        expandedNodes++;

        if (current.solution.size() == D) {
            if (current.g == clauseCount) {
                bestNode = current;
                found = true;
                break;
            }
            continue;
        }

        for (int val : {1, -1}) {
            Node next = current;
            next.solution.push_back(val);
            if (!isValidPartialAssignment(next.solution)) continue;

            next.g = satisfiedClauses(next.solution);
            next.h = heuristic(next.solution);

            string code = encodeState(next.solution);
            if (visited.count(code)) continue;
            visited.insert(code);
            openList.push(next);
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    string resultFile = (outDir / ("Result_Dim=" + to_string(D) + ".txt")).string();
    ofstream fout(resultFile);

    if (found) {
        fout << "-- Answer Found:" << endl;
        for (int i = 0; i < D; i++)
            fout << "\tx" << (i + 1) << ": " << (bestNode.solution[i] == 1 ? "True" : "False") << endl;

        fout << "-- Vector View: \n\t{ ";
        for (int i = 0; i < D; i++)
            fout << (bestNode.solution[i] == 1 ? "1 " : "0 ");
        fout << "}" << endl;

        fout << "-- Cost(Total Clause Checks):\n\t" << totalClauseChecks  << endl;
    } else {
        fout << "-- No answer found.\n";
        fout << "-- Best Cost Found:\n\t" << bestNode.g << " / " << clauseCount << endl;
    }

    fout << "-- Expanded Nodes:\n\t" << expandedNodes << endl;
    fout << "-- Running Time:\n\t" << duration.count() << " ms" << endl;

    fout.close();
    cout << "Answer had written in " << resultFile << endl;
    return 0;
}
