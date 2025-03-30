#include "common.h"

int solution[100];
bool found = false;

bool dfs_checkClause(int a, int b, int c) {
    totalClauseChecks++;
    int valA = (a > 0) ? solution[a - 1] : -solution[-a - 1];
    int valB = (b > 0) ? solution[b - 1] : -solution[-b - 1];
    int valC = (c > 0) ? solution[c - 1] : -solution[-c - 1];
    return (valA == 1 || valB == 1 || valC == 1);
}

bool dfs_isClauseStillPossible(int a, int b, int c, int depth) {
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
        if ((lit > 0 && val == 1) || (lit < 0 && val == -1)) return true;
    }
    return unknown;
}

bool dfs_earlyPruningFails(int depth) {
    for (int i = 0; i < clauseCount; i++) {
        if (!dfs_isClauseStillPossible(clauses[i][0], clauses[i][1], clauses[i][2], depth))
            return true;
    }
    return false;
}

int dfs_satisfiedClauses() {
    int count = 0;
    for (int i = 0; i < clauseCount; i++)
        if (dfs_checkClause(clauses[i][0], clauses[i][1], clauses[i][2]))
            count++;
    return count;
}

bool dfs(int index) {
    expandedNodes++;
    if (index == D) {
        if (dfs_satisfiedClauses() == clauseCount) {
            found = true;
            return true;
        }
        return false;
    }

    for (int val : {1, -1}) {
        solution[index] = val;
        if (!dfs_earlyPruningFails(index + 1)) {
            if (dfs(index + 1)) return true;
        }
    }

    return false;
}

int dfs_main() {
    auto start = high_resolution_clock::now();
    string fileName = getInputFileName();
    if (!loadClauses(fileName)) return 1;

    bool isFound = dfs(0);
    int cost = dfs_satisfiedClauses();

    fs::path outDir = "results/DFS";
    if (!fs::exists(outDir)) fs::create_directories(outDir);
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
        fout << "No solution found.\n";
    }

    fout << "-- Cost (Total Clause Checks):\n\t" << totalClauseChecks << endl;
    fout << "-- Expanded Nodes:\n\t" << expandedNodes << endl;
    auto end = high_resolution_clock::now();
    fout << "-- Running Time:\n\t" << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    fout.close();
    cout << "Answer had written in " << resultFile << endl;
    return 0;
}
