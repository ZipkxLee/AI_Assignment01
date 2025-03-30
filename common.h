#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <queue>
#include <unordered_set>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

extern int D;
extern int clauses[1000][3];
extern int clauseCount;
extern long long expandedNodes;
extern long long totalClauseChecks;

// ========== 通用工具函式 ==========
int catchDimFromFilename(const string& filename);
string getInputFileName();
bool loadClauses(const string& filename);
bool checkClause(const vector<int>& sol, int a, int b, int c, long long& counter);


#endif // COMMON_H_INCLUDED
