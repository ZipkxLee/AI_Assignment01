#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <filesystem>
#include <chrono>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

int D = 0, clauseCount = 0;
int clauses[1000][3];
int solution[100];
long long expandedNodes = 0;
long long totalClauseChecks = 0;  // ✓ Total checks in all stages
bool found = false;

int catchDimFromFilename(const string& filename) {
    size_t pos = filename.find("Dim=");
    if (pos == string::npos) return -1;
    pos += 4;
    size_t end = filename.find_first_not_of("0123456789", pos);
    string dimStr = filename.substr(pos, end - pos);
    return stoi(dimStr);
}

bool checkClause(int a, int b, int c) {
    totalClauseChecks++;
    int valA = (a > 0) ? solution[a - 1] : -solution[-a - 1];
    int valB = (b > 0) ? solution[b - 1] : -solution[-b - 1];
    int valC = (c > 0) ? solution[c - 1] : -solution[-c - 1];
    return (valA == 1 || valB == 1 || valC == 1);
}

bool isClauseStillPossible(int a, int b, int c, int depth) {
    totalClauseChecks++;
    bool unknown = false;
    for (int i = 0; i < 3; ++i) {
        int lit = (i == 0) ? a : (i == 1) ? b : c;
        int idx = abs(lit) - 1;
        if (idx >= depth) {
            unknown = true;
            continue;
        }
        int val = solution[idx];
        if ((lit > 0 && val == 1) || (lit < 0 && val == -1)) {
            return true;
        }
    }
    return unknown;
}

bool earlyPruningFails(int depth) {
    for (int i = 0; i < clauseCount; ++i) {
        if (!isClauseStillPossible(clauses[i][0], clauses[i][1], clauses[i][2], depth))
            return true;
    }
    return false;
}

int satisfiedClauses() {
    int satisfied = 0;
    for (int i = 0; i < clauseCount; i++)
        if (checkClause(clauses[i][0], clauses[i][1], clauses[i][2]))
            satisfied++;
    return satisfied;
}

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
    auto start = high_resolution_clock::now();
    string fileName;
    string defaultPath = "./data/3SAT_Dim=10.csv";
    cout << "CSV file path ( press enter for the default path: \"" << defaultPath << "\"): ";
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

    bool isFound = dfs(0);
    int cost = satisfiedClauses();

    string resultFile = (outDir / ("Result_Dim=" + to_string(D) + ".txt")).string();
    ofstream fout(resultFile);

    if (isFound) {
        fout << "Answer Found:\n";
        for (int i = 0; i < D; i++)
            fout << "\tx" << (i + 1) << ": " << (solution[i] == 1 ? "True" : "False") << endl;

        fout << "-- Vector View:\n\t{ ";
        for (int i = 0; i < D; i++)
            fout << (solution[i] == 1 ? "1 " : "0 ");
        fout << "}" << endl;
    } else {
        fout << "No solution found." << endl;
    }

    fout << "-- Cost (Total Clause Checks):\n\t" << totalClauseChecks << endl;
    fout << "-- Expanded Nodes:\n\t" << expandedNodes << endl;

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    fout << "-- Running Time:\n\t" << duration.count() << " ms" << endl;

    fout.close();
    cout << "Answer had written in " << resultFile << endl;
    return 0;
}
