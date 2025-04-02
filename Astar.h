#ifndef ASTAR_H
#define ASTAR_H

#include "common.h"
#include <vector>
#include <queue>
#include <functional>
#include <chrono>

using namespace std;

struct AStarNode {
    Assignment assign;
    int idx;
    int g;
    int h;
    int f;
};

inline int computeHeuristicAStar(const vector<Clause>& clauses, const Assignment &assign, int dim, long long &clauseChecks) {
    int totalWeight = 0;
    for (const Clause &clause : clauses) {
        bool sat = false;
        int unassignedCount = 0;
        for (int j = 0; j < 3; j++) {
            int var = abs(clause.lit[j]) - 1;
            clauseChecks++;
            if (assign[var] == -1)
                unassignedCount++;
            else if ((clause.lit[j] > 0 && assign[var] == 1) ||
                     (clause.lit[j] < 0 && assign[var] == 0)) {
                sat = true;
                break;
            }
        }
        if (!sat) {
            if (unassignedCount == 1)
                totalWeight += 10;
            else
                totalWeight += unassignedCount;
        }
    }
    return totalWeight;
}

inline int nextUnassignedAStar(int dim, const Assignment &assign) {
    for (int i = 0; i < dim; i++) {
        if (assign[i] == -1)
            return i;
    }
    return dim;
}

inline Assignment aStarSearch(int dim, const vector<Clause>& clauses, long long &totalClauseCost, int &expandedNodes, double &runningTime, const vector<int>& staticOrder) {
    totalClauseCost = 0;
    expandedNodes = 0;
    auto cmp = [](const AStarNode &a, const AStarNode &b) { return a.f > b.f; };
    priority_queue<AStarNode, vector<AStarNode>, decltype(cmp)> open(cmp);

    AStarNode root;
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
    root.g = 0;
    root.h = computeHeuristicAStar(clauses, root.assign, dim, totalClauseCost);
    root.f = root.g + root.h;
    open.push(root);

    auto startTime = chrono::high_resolution_clock::now();
    while (!open.empty()) {
        AStarNode current = open.top();
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
            AStarNode child;
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
            child.g = current.g + 1;
            child.h = computeHeuristicAStar(clauses, child.assign, dim, totalClauseCost);
            child.f = child.g + child.h;
            open.push(child);
        }
    }
    auto endTime = chrono::high_resolution_clock::now();
    runningTime = chrono::duration<double, milli>(endTime - startTime).count();
    return Assignment();
}

#endif // ASTAR_H
