#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int D = 0, clauseCount = 0;
int clauses[1000][3];
int solution[100];
long long expandedNodes = 0;
bool found = false;

// 從檔名中擷取 D 值（如 Dim=30）
int catchDimFromFilename(const string& filename) {
    size_t pos = filename.find("Dim=");
    if (pos == string::npos) return -1;
    pos += 4;
    size_t end = filename.find_first_not_of("0123456789", pos);
    string dimStr = filename.substr(pos, end - pos);
    return stoi(dimStr);
}

// Early Pruning：只在三個變數都已 assign 且都為 false 時才剪枝
bool isClauseStillPossible(int a, int b, int c, int depth) {
    bool unknown = false;
    for (int i = 0; i < 3; ++i) {
        int lit = (i == 0) ? a : (i == 1) ? b : c;
        int idx = abs(lit) - 1;
        if (idx >= depth) {
            unknown = true;  // 還沒 assign，不能下定論
            continue;
        }
        int val = solution[idx];
        if ((lit > 0 && val == 1) || (lit < 0 && val == -1)) {
            return true; // 至少一個為真，Clause 可滿足
        }
    }
    return unknown;  // 有未 assign → 保留可能性
}

// 若任何子句必定無法滿足，則剪枝
bool earlyPruningFails(int depth) {
    for (int i = 0; i < clauseCount; ++i) {
        if (!isClauseStillPossible(clauses[i][0], clauses[i][1], clauses[i][2], depth))
            return true;
    }
    return false;
}

// 檢查最終整組變數是否滿足所有子句
bool checkClause(int a, int b, int c) {
    int valA = (a > 0) ? solution[a - 1] : -solution[-a - 1];
    int valB = (b > 0) ? solution[b - 1] : -solution[-b - 1];
    int valC = (c > 0) ? solution[c - 1] : -solution[-c - 1];
    return (valA == 1 || valB == 1 || valC == 1);
}

int satisfiedClauses() {
    int satisfied = 0;
    for (int i = 0; i < clauseCount; i++)
        if (checkClause(clauses[i][0], clauses[i][1], clauses[i][2]))
            satisfied++;
    return satisfied;
}

// DFS + Early Pruning（原始變數順序）
bool dfs(int index) {
    expandedNodes++;
    if (index == D) {
        if (satisfiedClauses() == clauseCount) {
            found = true;
            return true;
        }
        return false;
    }

    for (int val : {1, -1}) {
        solution[index] = val;
        if (!earlyPruningFails(index + 1)) {
            if (dfs(index + 1)) return true;
        }
    }

    return false;
}

int main() {
    string fileName;
    string defaultPath = "./data/3SAT_Dim=10.csv"; 
    cout << "CSV file path ( press enter for the default path: \"" << defaultPath <<  "\"): ";
    getline(cin, fileName);
    if (fileName.empty()) fileName = defaultPath;
    
    fs::path outDir = "results/DFS";
    if (!fs::exists(outDir)) {
        fs::create_directories(outDir);
    }

    D = catchDimFromFilename(fileName);
    if (D <= 0) {
        cout << "Can't catch Dim from filename!" << endl;
        return 1;
    }

    ifstream fin(fileName);
    if (!fin) {
        cout << "Error opening file:" << fileName << endl;
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

    clock_t start = clock();
    bool isFound = dfs(0);
    clock_t end = clock();
    double runningTime = (double)(end - start) / CLOCKS_PER_SEC;
    int cost = satisfiedClauses();

    string resultFile = ( outDir / ("Result_Dim=" + to_string(D) + ".txt")).string();
    ofstream fout(resultFile);

    if (isFound) {
        fout << "Solution found:" << endl;
        for (int i = 0; i < D; i++)
            fout << "x" << (i + 1) << ": " << (solution[i] == 1 ? "True" : "False") << endl;
    } else {
        fout << "No solution found." << endl;
    }

    fout << "Cost (Satisfied clauses): " << cost << " / " << clauseCount << endl;
    fout << "Expanded Nodes: " << expandedNodes << endl;
    fout << "Running Time: " << runningTime << " sec" << endl;
    fout.close();

    cout << "Results written to " << resultFile << endl;
    return 0;
}
