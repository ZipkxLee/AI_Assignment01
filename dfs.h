#ifndef DFS_H
#define DFS_H

#include "common.h"
#include <vector>
#include <chrono>

using namespace std;

bool dfsSolve(int dim, const vector<Clause>& clauses, Assignment &assign, long long &clauseChecks, int &expandedNodes, const vector<int>& staticOrder) {
    if (!unitPropagation(clauses, assign, clauseChecks))
        return false;

    bool complete = true;
    for (int i = 0; i < dim; i++) {
        if (assign[i] == -1) {
            complete = false;
            break;
        }
    }
    if (complete)
        return allClausesSatisfied(clauses, assign, clauseChecks);

    int idx = selectCombinedVariable(clauses, assign, dim, clauseChecks, staticOrder);
    if (idx == -1)
        return false;

    for (int val = 0; val < 2; val++) {
        Assignment newAssign = assign;
        newAssign[idx] = val;
        expandedNodes++;
        if (dfsSolve(dim, clauses, newAssign, clauseChecks, expandedNodes, staticOrder)) {
            assign = newAssign;
            return true;
        }
    }
    return false;
}

inline Assignment dfsSearch(int dim, const vector<Clause>& clauses, long long &totalClauseCost, int &expandedNodes, double &runningTime, const vector<int>& staticOrder) {
    Assignment assign(dim, -1);
    totalClauseCost = 0;
    expandedNodes = 0;
    auto start = chrono::high_resolution_clock::now();
    bool found = dfsSolve(dim, clauses, assign, totalClauseCost, expandedNodes, staticOrder);
    auto end = chrono::high_resolution_clock::now();
    runningTime = chrono::duration<double, milli>(end - start).count();
    if (found)
        return assign;
    return Assignment();
}

#endif // DFS_H
