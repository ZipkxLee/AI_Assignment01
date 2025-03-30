#include "common.h"

int D = 0;
int clauses[1000][3];
int clauseCount = 0;
long long expandedNodes = 0;
long long totalClauseChecks = 0;

int catchDimFromFilename(const string& filename) {
    size_t pos = filename.find("Dim=");
    if (pos == string::npos) return -1;
    pos += 4;
    size_t end = filename.find_first_not_of("0123456789", pos);
    string dimStr = filename.substr(pos, end - pos);
    return stoi(dimStr);
}

string getInputFileName() {
    string fileName;
    string defaultPath = "./data/3SAT_Dim=10.csv";
    cout << "CSV file path (press enter for default \"" << defaultPath << "\"): ";
    getline(cin, fileName);
    return fileName.empty() ? defaultPath : fileName;
}

bool loadClauses(const string& filename) {
    clauseCount = 0;
    D = catchDimFromFilename(filename);
    if (D <= 0) {
        cout << "Can't extract Dim from filename!" << endl;
        return false;
    }
    ifstream fin(filename);
    if (!fin) {
        cout << "Error opening file: " << filename << endl;
        return false;
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
    return true;
}

bool checkClause(const vector<int>& sol, int a, int b, int c, long long& counter) {
    counter++;
    auto getVal = [&](int var) -> int {
        int idx = abs(var) - 1;
        if (idx >= sol.size()) return 0;
        int val = sol[idx];
        return (var > 0) ? val : -val;
    };
    return (getVal(a) == 1 || getVal(b) == 1 || getVal(c) == 1);
}
