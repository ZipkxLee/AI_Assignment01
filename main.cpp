#include <iostream>
using namespace std;

// 宣告外部函式
void dfs_main();
void bfs_main();
void astar_main();
void runVerifier();

int main() {
    cout << "==== 3SAT Solver ====" << endl;

    int choice;
    cout << "Choose algorithm:\n"
         << "  1. DFS\n"
         << "  2. BFS\n"
         << "  3. A* Search\n"
         << "  4. Verify Result\n"
         << "Enter your choice (1-4): ";

    cin >> choice;
    cin.ignore();  //  清除 cin 留下的 \n

    switch (choice) {
        case 1:
            dfs_main();  // 在 DFS.h 中宣告 dfs_main()
            break;
        case 2:
            bfs_main();  // 在 BFS.h 中宣告 bfs_main()
            break;
        case 3:
            astar_main(); // 在 Astar.h 中宣告 astar_main()
            break;
        case 4:
            runVerifier();
            break;
        default:
            cout << "Invalid choice." << endl;
    }

    return 0;
}
