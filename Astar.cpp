#include "common.h"

struct astar_Node {
    vector<int> solution;
    int g, h;
    int f() const { return g + h; }
    bool operator<(const astar_Node& other) const { return f() > other.f(); }
};

int astar_satisfiedClauses(const vector<int>& sol) {
    int count = 0;
    for (int i = 0; i < clauseCount; i++)
        if (checkClause(sol, clauses[i][0], clauses[i][1], clauses[i][2], totalClauseChecks))
            count++;
    return count;
}

int astar_heuristic(const vector<int>& sol) {
    vector<int> tmp = sol;
    tmp.resize(D, 0);
    return clauseCount - astar_satisfiedClauses(tmp);
}

bool astar_isValidPartialAssignment(const vector<int>& sol) {
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

string astar_encodeState(const vector<int>& sol) {
    string code;
    for (int v : sol) code += (v == 1 ? '1' : '0');
    return code;
}

int astar_main() {
    auto start = high_resolution_clock::now();
    string fileName = getInputFileName();
    if (!loadClauses(fileName)) return 1;

    priority_queue<astar_Node> openList;
    unordered_set<string> visited;

    astar_Node startNode{{}, 0, astar_heuristic({})};
    openList.push(startNode);
    visited.insert(astar_encodeState(startNode.solution));

    astar_Node bestNode;
    bool found = false;

    while (!openList.empty()) {
        astar_Node current = openList.top();
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
            astar_Node next = current;
            next.solution.push_back(val);
            if (!astar_isValidPartialAssignment(next.solution)) continue;

            next.g = astar_satisfiedClauses(next.solution);
            next.h = astar_heuristic(next.solution);

            string code = astar_encodeState(next.solution);
            if (visited.count(code)) continue;
            visited.insert(code);
            openList.push(next);
        }
    }

    fs::path outDir = "results/Astar";
    if (!fs::exists(outDir)) fs::create_directories(outDir);
    string resultFile = (outDir / ("Result_Dim=" + to_string(D) + ".txt")).string();
    ofstream fout(resultFile);

    if (found) {
        fout << "-- Answer Found:\n";
        for (int i = 0; i < D; i++)
            fout << "\tx" << (i + 1) << ": " << (bestNode.solution[i] == 1 ? "True" : "False") << endl;
        fout << "-- Vector View:\n\t{ ";
        for (int i = 0; i < D; i++)
            fout << (bestNode.solution[i] == 1 ? "1 " : "0 ");
        fout << "}" << endl;
        fout << "-- Cost(Total Clause Checks):\n\t" << totalClauseChecks << endl;
    } else {
        fout << "-- No answer found.\n";
        fout << "-- Best Cost Found:\n\t" << bestNode.g << " / " << clauseCount << endl;
    }

    fout << "-- Expanded Nodes:\n\t" << expandedNodes << endl;
    auto end = high_resolution_clock::now();
    fout << "-- Running Time:\n\t" << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
    fout.close();

    cout << "Answer had written in " << resultFile << endl;
    return 0;
}
