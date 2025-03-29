#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int D;
int clauses[1000][3];
int clauseCount = 0;
long long expandedNodes = 0;
const int maxOpenSize = 2048;


struct Node {
    vector<int> solution;
    int heuristicVal;
    bool operator<(const Node& other) const {
        return heuristicVal < other.heuristicVal;
    }
};

int catchDimFromFilename(const string& filename) {
    size_t pos = filename.find("Dim=");
    if (pos == string::npos) return -1;
    pos += 4;
    size_t end = filename.find_first_not_of("0123456789", pos);
    string dimStr = filename.substr(pos, end - pos);
    return stoi(dimStr);
}

bool checkClause(const vector<int>& sol, int a, int b, int c) {
    auto getVal = [&](int var) -> int {
        int idx = abs(var) - 1;
        if (idx >= sol.size()) return 0;  // 未指定 → 回傳 false
        int val = sol[idx];
        return (var > 0) ? val : -val;
    };
    return (getVal(a) == 1 || getVal(b) == 1 || getVal(c) == 1);
}

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

int satisfiedClauses(const vector<int>& sol) {
    int count = 0;
    for (int i = 0; i < clauseCount; i++)
        if (checkClause(sol, clauses[i][0], clauses[i][1], clauses[i][2]))
            count++;
    return count;
}

int heuristic(const vector<int>& sol) {
    vector<int> tmp = sol;
    tmp.resize(D, 0);
    return satisfiedClauses(tmp);
}

int main() {
    string fileName;
    string defaultPath = "./data/3SAT_Dim=10.csv"; 
    cout << "CSV file path ( press enter for the default path: \"" << defaultPath <<  "\"): ";
    getline(cin, fileName);
    if (fileName.empty()) fileName = defaultPath;

    fs::path outDir = "results/BFS";
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

    priority_queue<Node> openList;
    openList.push({{}, heuristic({})});
    vector<int> bestSolution;
    int bestHeuristic = -1;
    bool found = false;

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();
        expandedNodes++;

        if (current.solution.size() == D) {
            int sat = satisfiedClauses(current.solution);
            if (sat == clauseCount) {
                bestSolution = current.solution;
                found = true;
                break;
            }
            if (sat > bestHeuristic) {
                bestHeuristic = sat;
                bestSolution = current.solution;
            }
            continue;
        }

        // expand True
        vector<int> solTrue = current.solution;
        solTrue.push_back(1);
        if (isValidPartialAssignment(solTrue))
            openList.push({solTrue, heuristic(solTrue)});

        // expand False
        vector<int> solFalse = current.solution;
        solFalse.push_back(-1);
        if (isValidPartialAssignment(solFalse))
            openList.push({solFalse, heuristic(solFalse)});

        while (openList.size() > maxOpenSize) openList.pop();
    }

    clock_t end = clock();
    double runningTime = (double)(end - start) / CLOCKS_PER_SEC;

    string resultFile = ( outDir / ("Result_Dim=" + to_string(D) + ".txt")).string();
    ofstream fout(resultFile);

    if (found) {
        fout << "Solution found:\n";
        for (int i = 0; i < D; i++)
            fout << "x" << (i + 1) << ": " << (bestSolution[i] == 1 ? "True" : "False") << endl;
        fout << "Cost (Satisfied clauses): " << clauseCount << " / " << clauseCount << endl;
    } else {
        fout << "No solution found.\n";
        fout << "Best Heuristic Found: " << bestHeuristic << " / " << clauseCount << endl;
    }

    fout << "Expanded Nodes: " << expandedNodes << endl;
    fout << "Running Time: " << runningTime << " sec" << endl;
    fout.close();

    cout << "Results written to " << resultFile << endl;
    return 0;
}
