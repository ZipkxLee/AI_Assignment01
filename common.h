#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

// Assignment type: -1 = unassigned, 0 = false, 1 = true.
typedef vector<int> Assignment;

// Clause structure: each clause consists of 3 literals.
// A positive number represents a positive literal; a negative number represents a negated literal.
struct Clause {
    int lit[3];
};

// Parse the dimension value from the filename.
// For example, "3SAT_Dim=10.csv" returns 10.
inline int readDimensionFromFilename(const string &filename) {
    size_t pos = filename.find("Dim=");
    if (pos != string::npos) {
        size_t start = pos + 4;
        size_t end = filename.find_first_not_of("0123456789", start);
        string dimStr = filename.substr(start, end - start);
        return stoi(dimStr);
    }
    return -1;
}

// Read clause data from a CSV file.
// The first line "Dim=x" is ignored; each subsequent line has 3 comma-separated numbers.
inline vector<Clause> readClauses(const string &filename) {
    vector<Clause> clauses;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Unable to open file: " << filename << endl;
        return clauses;
    }
    string line;
    if (!getline(infile, line))
        return clauses;
    while (getline(infile, line)) {
        if (line.empty())
            continue;
        istringstream iss(line);
        string token;
        Clause clause;
        int i = 0;
        while (getline(iss, token, ',')) {
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            clause.lit[i] = stoi(token);
            i++;
            if (i >= 3) break;
        }
        if (i == 3)
            clauses.push_back(clause);
    }
    infile.close();
    return clauses;
}

// Write the solution and statistics into the specified output file.
inline void writeResult(const Assignment &assign,
                        const string &filename,
                        long long totalClauseCost,
                        int expandedNodes,
                        double runningTime) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Unable to open output file: " << filename << endl;
        return;
    }
    outfile << "-- Answer Found:\n";
    for (size_t i = 0; i < assign.size(); i++) {
        outfile << "\tx" << (i + 1) << ": " << ((assign[i] == 1) ? "True" : "False") << "\n";
    }
    outfile << "-- Vector View:\n\t{ ";
    for (auto val : assign) {
        outfile << ((val == 1) ? "1 " : "0 ");
    }
    outfile << "}\n";
    outfile << "-- Total Clause Cost:\n\t" << totalClauseCost << "\n";
    outfile << "-- Expanded Nodes:\n\t" << expandedNodes << "\n";
    outfile << "-- Running Time:\n\t" << runningTime << " ms\n";
    outfile.close();
}

// Compute a static variable ordering based on frequency (descending order).
inline vector<int> computeStaticOrder(const vector<Clause>& clauses, int dim) {
    vector<int> frequency(dim, 0);
    for (const Clause &clause : clauses) {
        for (int j = 0; j < 3; j++) {
            int var = abs(clause.lit[j]) - 1;
            frequency[var]++;
        }
    }
    vector<int> order(dim);
    for (int i = 0; i < dim; i++) {
        order[i] = i;
    }
    sort(order.begin(), order.end(), [&](int a, int b) {
        return frequency[a] > frequency[b];
    });
    return order;
}

inline int selectCombinedVariable(const vector<Clause>& clauses, const Assignment &assign, int dim, long long &clauseChecks, const vector<int>& staticOrder) {
    int unassignedCount = 0;
    for (int i = 0; i < dim; i++) {
        if (assign[i] == -1)
            unassignedCount++;
    }
    if (unassignedCount > (int)(0.8 * dim)) {
        for (int idx : staticOrder) {
            if (assign[idx] == -1)
                return idx;
        }
    }

    int bestIdx = -1;
    int bestScore = -1;
    for (int i = 0; i < dim; i++) {
        if (assign[i] == -1) {
            int score = 0;
            for (const Clause &clause : clauses) {
                bool satisfied = false;
                for (int j = 0; j < 3; j++) {
                    int var = abs(clause.lit[j]) - 1;
                    clauseChecks++;
                    if (assign[var] != -1) {
                        if ((clause.lit[j] > 0 && assign[var] == 1) ||
                            (clause.lit[j] < 0 && assign[var] == 0)) {
                            satisfied = true;
                            break;
                        }
                    }
                }
                if (!satisfied) {
                    for (int j = 0; j < 3; j++) {
                        int var = abs(clause.lit[j]) - 1;
                        if (var == i) {
                            score++;
                            break;
                        }
                    }
                }
            }
            if (score > bestScore) {
                bestScore = score;
                bestIdx = i;
            }
        }
    }
    if (bestIdx == -1) {
        for (int i = 0; i < dim; i++) {
            if (assign[i] == -1)
                return i;
        }
    }
    return bestIdx;
}

inline bool unitPropagation(const vector<Clause>& clauses, Assignment &assign, long long &clauseChecks) {
    bool updated = true;
    int dim = assign.size();
    while (updated) {
        updated = false;
        for (const Clause &clause : clauses) {
            bool satisfied = false;
            for (int j = 0; j < 3; j++) {
                int var = abs(clause.lit[j]) - 1;
                if (assign[var] != -1) {
                    if ((clause.lit[j] > 0 && assign[var] == 1) ||
                        (clause.lit[j] < 0 && assign[var] == 0)) {
                        satisfied = true;
                        break;
                    }
                }
            }
            if (satisfied)
                continue;
            int unassignedCount = 0;
            int lastUnassigned = -1;
            for (int j = 0; j < 3; j++) {
                int var = abs(clause.lit[j]) - 1;
                clauseChecks++;
                if (assign[var] == -1) {
                    unassignedCount++;
                    lastUnassigned = var;
                }
            }
            if (unassignedCount == 0)
                return false; // Conflict.
            if (unassignedCount == 1) {
                for (int j = 0; j < 3; j++) {
                    int var = abs(clause.lit[j]) - 1;
                    if (var == lastUnassigned) {
                        int value = (clause.lit[j] > 0) ? 1 : 0;
                        if (assign[var] == -1) {
                            assign[var] = value;
                            updated = true;
                        } else if (assign[var] != value) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

// Check if all clauses are satisfied.
inline bool allClausesSatisfied(const vector<Clause>& clauses, const Assignment &assign, long long &clauseChecks) {
    for (const Clause &clause : clauses) {
        bool satisfied = false;
        for (int j = 0; j < 3; j++) {
            int var = abs(clause.lit[j]) - 1;
            clauseChecks++;
            if (assign[var] != -1 && ((clause.lit[j] > 0 && assign[var] == 1) ||
                (clause.lit[j] < 0 && assign[var] == 0))) {
                satisfied = true;
                break;
            }
        }
        if (!satisfied)
            return false;
    }
    return true;
}

#endif // COMMON_H
