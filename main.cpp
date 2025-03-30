#include <iostream>
using namespace std;

// �ŧi�~���禡
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
    cin.ignore();  //  �M�� cin �d�U�� \n

    switch (choice) {
        case 1:
            dfs_main();  // �b DFS.h ���ŧi dfs_main()
            break;
        case 2:
            bfs_main();  // �b BFS.h ���ŧi bfs_main()
            break;
        case 3:
            astar_main(); // �b Astar.h ���ŧi astar_main()
            break;
        case 4:
            runVerifier();
            break;
        default:
            cout << "Invalid choice." << endl;
    }

    return 0;
}
