#include <iostream>
#include <string>
#include <vector>
#include "common.h"
#include "dfs.h"
#include "bfs.h"
#include "astar.h"
#include <filesystem>
#include <limits>

namespace fs = std::filesystem;
using namespace std;

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

int main() {
    while (true) {
        cout << "\nSelect the algorithm:" << endl;
        cout << "1: DFS" << endl;
        cout << "2: BFS" << endl;
        cout << "3: A*" << endl;
        cout << "4: Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 4) {
            cout << "Exiting program." << endl;
            break;
        } else if (choice < 1 || choice > 4) {
            cerr << "Invalid input! Please enter 1, 2, 3, or 4." << endl;
            continue;
        }

        string filename;
        cout << "\nEnter the data file path (default: ./datas/3SAT_Dim=30.csv): ";
        getline(cin, filename);
        if (filename.empty())
            filename = "./datas/3SAT_Dim=30.csv";

        int dim = readDimensionFromFilename(filename);
        if (dim <= 0) {
            cerr << "Failed to read a valid Dimension value from filename." << endl;
            continue;
        }

        vector<Clause> clauses = readClauses(filename);
        if (clauses.empty()) {
            cerr << "No clause data was read." << endl;
            continue;
        }

        fs::create_directories("results/DFS");
        fs::create_directories("results/BFS");
        fs::create_directories("results/Astar");

        vector<int> staticOrder = computeStaticOrder(clauses, dim);

        long long totalClauseCost = 0;
        int expandedNodes = 0;
        double runningTime = 0.0;
        Assignment solution;
        string outFile;

        if (choice == 1) {
            solution = dfsSearch(dim, clauses, totalClauseCost, expandedNodes, runningTime, staticOrder);
            outFile = "results/DFS/result_Dim=" + to_string(dim) + ".txt";
            writeResult(solution, outFile, totalClauseCost, expandedNodes, runningTime);
            system(CLEAR_COMMAND);
            cout << "DFS search complete. Results output to " << outFile << endl;
        } else if (choice == 2) {
            solution = bfsSearch(dim, clauses, totalClauseCost, expandedNodes, runningTime, staticOrder);
            outFile = "results/BFS/result_Dim=" + to_string(dim) + ".txt";
            writeResult(solution, outFile, totalClauseCost, expandedNodes, runningTime);
            system(CLEAR_COMMAND);
            cout << "BFS search complete. Results output to " << outFile << endl;
        } else if (choice == 3) {
            solution = aStarSearch(dim, clauses, totalClauseCost, expandedNodes, runningTime, staticOrder);
            outFile = "results/Astar/result_Dim=" + to_string(dim) + ".txt";
            writeResult(solution, outFile, totalClauseCost, expandedNodes, runningTime);
            system(CLEAR_COMMAND);
            cout << "A* search complete. Results output to " << outFile << endl;
        }
    }

    return 0;
}
