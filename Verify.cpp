#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int D;
vector<vector<int>> clauses;

// Extract D from filename (e.g., "Dim=30")
int extractDimFromFilename(const string& filename) {
    size_t pos = filename.find("Dim=");
    if (pos == string::npos) return -1;
    pos += 4;
    size_t end = filename.find_first_not_of("0123456789", pos);
    return stoi(filename.substr(pos, end - pos));
}

// Parse Vector View from result file
vector<int> parseVectorView(const string& resultFile) {
    ifstream fin(resultFile);
    vector<int> vec;
    string line;
    while (getline(fin, line)) {
        if (line.find("-- Vector View:") != string::npos) {
            getline(fin, line); // next line
            size_t left = line.find("{");
            size_t right = line.find("}");
            if (left == string::npos || right == string::npos) break;
            string content = line.substr(left + 1, right - left - 1);
            stringstream ss(content);
            int bit;
            while (ss >> bit) {
                vec.push_back(bit);
            }
            break;
        }
    }
    return vec;
}

// Check whether a single clause is satisfied
bool checkClause(const vector<int>& sol, int a, int b, int c) {
    auto getVal = [&](int var) -> int {
        int idx = abs(var) - 1;
        int val = sol[idx];
        return (var > 0) ? val : 1 - val;  // 1 = true, 0 = false
    };
    return (getVal(a) == 1 || getVal(b) == 1 || getVal(c) == 1);
}

int main() {
    string dataFile, resultFile;
    cout << "Enter CSV clause file path (e.g., ./data/3SAT_Dim=10.csv): ";
    getline(cin, dataFile);

    cout << "Enter result file path (e.g., ./results/DFS/Result_Dim=10.txt): ";
    getline(cin, resultFile);

    D = extractDimFromFilename(resultFile);
    if (D <= 0) {
        cout << "Error: Failed to extract Dim from result filename." << endl;
        return 1;
    }

    vector<int> solution = parseVectorView(resultFile);
    if (solution.size() != D) {
        cout << "Error: Invalid vector length in result file." << endl;
        return 1;
    }

    ifstream fin(dataFile);
    if (!fin) {
        cout << "Error: Failed to open clause file." << endl;
        return 1;
    }

    int a, b, c;
    char comma;
    while (fin >> a >> comma >> b >> comma >> c) {
        clauses.push_back({a, b, c});
    }

    int failed = 0;
    for (int i = 0; i < clauses.size(); i++) {
        if (!checkClause(solution, clauses[i][0], clauses[i][1], clauses[i][2])) {
            cout << "Clause " << (i + 1) << " NOT satisfied: (" 
                 << clauses[i][0] << ", " << clauses[i][1] << ", " << clauses[i][2] << ")" << endl;
            failed++;
        }
    }

    cout << "\n--- Verification Report ---" << endl;
    cout << "Satisfied Clauses: " << (clauses.size() - failed) 
         << " / " << clauses.size() << endl;

    if (failed == 0)
        cout << " All clauses satisfied. VALID solution." << endl;
    else
        cout << failed << " clause(s) not satisfied. INVALID solution." << endl;

    return 0;
}
