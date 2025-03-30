#include "common.h"


struct bfs_Node {
    vector<int> solution;
    int heuristicVal;
    bool operator<(const bfs_Node& other) const {
        return heuristicVal < other.heuristicVal;
    }
};

bool bfs_isValidPartialAssignment(const vector<int>& sol) {
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

int bfs_satisfiedClauses(const vector<int>& sol) {
    int count = 0;
    for (int i = 0; i < clauseCount; i++)
        if (checkClause(sol, clauses[i][0], clauses[i][1], clauses[i][2], totalClauseChecks))
            count++;
    return count;
}

int bfs_heuristic(const vector<int>& sol) {
    vector<int> tmp = sol;
    tmp.resize(D, 0);
    return bfs_satisfiedClauses(tmp);
}

int bfs_main() {
    auto start = high_resolution_clock::now();
    string fileName = getInputFileName();
    if (!loadClauses(fileName)) return 1;

    priority_queue<bfs_Node> openList;
    openList.push({{}, bfs_heuristic({})});
    vector<int> bestSolution;
    int bestHeuristic = -1;
    bool found = false;

    while (!openList.empty()) {
        bfs_Node current = openList.top();
        openList.pop();
        expandedNodes++;

        if (current.solution.size() == D) {
            int sat = bfs_satisfiedClauses(current.solution);
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

        for (int val : {1, -1}) {
            vector<int> sol = current.solution;
            sol.push_back(val);
            if (bfs_isValidPartialAssignment(sol))
                openList.push({sol, bfs_heuristic(sol)});
        }
    }

    fs::path outDir = "results/BFS";
    if (!fs::exists(outDir)) fs::create_directories(outDir);
    string resultFile = (outDir / ("Result_Dim=" + to_string(D) + ".txt")).string();
    ofstream fout(resultFile);

    if (found) {
        fout << "-- Answer found:\n";
        for (int i = 0; i < D; i++)
            fout << "\tx" << (i + 1) << ": " << (bestSolution[i] == 1 ? "True" : "False") << endl;
        fout << "-- Vector View:\n\t{ ";
        for (int i = 0; i < D; i++)
            fout << (bestSolution[i] == 1 ? "1 " : "0 ");
        fout << "}" << endl;
        fout << "-- Cost(Total Clause Checks):\n\t" << totalClauseChecks << endl;
    } else {
        fout << "No solution found.\n";
        fout << "-- Best Heuristic Found:\n\t" << bestHeuristic << " / " << clauseCount << endl;
        fout << "-- Cost(Total Clause Checks):\n\t" << totalClauseChecks << endl;
    }

    fout << "-- Expanded Nodes:\n\t" << expandedNodes << endl;
    auto end = high_resolution_clock::now();
    fout << "-- Running Time:\n\t" << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    fout.close();
    cout << "Answer had written in " << resultFile << endl;
    return 0;
}
