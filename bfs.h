#ifndef BFS_H
#define BFS_H

#include "common.h"
#include <vector>
#include <queue>
#include <functional>
#include <chrono>

using namespace std;

struct BFSNode {
    Assignment assign;
    int idx;
    int h;
};

inline int computeHeuristic(const vector<Clause>& clauses, const Assignment &assign, int dim, long long &clauseChecks) {
    int totalWeight = 0;
    for (const Clause &clause : clauses) {
        bool satisfied = false;
        int unassignedCount = 0;
        for (int j = 0; j < 3; j++) {
            int var = abs(clause.lit[j]) - 1;
            clauseChecks++;
            if (assign[var] == -1)
                unassignedCount++;
            else if ((clause.lit[j] > 0 && assign[var] == 1) ||
                     (clause.lit[j] < 0 && assign[var] == 0)) {
                satisfied = true;
                break;
            }
        }
        if (!satisfied) {
            if (unassignedCount == 1)
                totalWeight += 10;
            else
                totalWeight += unassignedCount;
        }
    }
    return totalWeight;
}

inline int nextUnassigned(int dim, const Assignment &assign) {
    for (int i = 0; i < dim; i++) {
        if (assign[i] == -1)
            return i;
    }
    return dim;
}

inline Assignment bfsSearch(int dim, const vector<Clause>& clauses, long long &totalClauseCost, int &expandedNodes, double &runningTime, const vector<int>& staticOrder) {
    totalClauseCost = 0;
    expandedNodes = 0;
    auto cmp = [](const BFSNode &a, const BFSNode &b) { return a.h > b.h; };
    priority_queue<BFSNode, vector<BFSNode>, decltype(cmp)> open(cmp);

    BFSNode root;
    root.assign = Assignment(dim, -1);
    unitPropagation(clauses, root.assign, totalClauseCost);
    bool complete = true;
    for (int i = 0; i < dim; i++) {
        if (root.assign[i] == -1) { complete = false; break; }
    }
    if (complete)
        root.idx = dim;
    else
        root.idx = selectCombinedVariable(clauses, root.assign, dim, totalClauseCost, staticOrder);
    root.h = computeHeuristic(clauses, root.assign, dim, totalClauseCost);
    open.push(root);

    auto startTime = chrono::high_resolution_clock::now();
    while (!open.empty()) {
        BFSNode current = open.top();
        open.pop();
        expandedNodes++;
        if (current.idx >= dim) {
            if (allClausesSatisfied(clauses, current.assign, totalClauseCost)) {
                auto endTime = chrono::high_resolution_clock::now();
                runningTime = chrono::duration<double, milli>(endTime - startTime).count();
                return current.assign;
            }
            continue;
        }
        for (int val = 0; val < 2; val++) {
            BFSNode child;
            child.assign = current.assign;
            child.assign[current.idx] = val;
            unitPropagation(clauses, child.assign, totalClauseCost);
            bool comp = true;
            for (int i = 0; i < dim; i++) {
                if (child.assign[i] == -1) { comp = false; break; }
            }
            if (comp)
                child.idx = dim;
            else
                child.idx = selectCombinedVariable(clauses, child.assign, dim, totalClauseCost, staticOrder);
            child.h = computeHeuristic(clauses, child.assign, dim, totalClauseCost);
            open.push(child);
        }
    }
    auto endTime = chrono::high_resolution_clock::now();
    runningTime = chrono::duration<double, milli>(endTime - startTime).count();
    return Assignment();
}

#endif // BFS_H
