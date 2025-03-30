AI_Assignment01 – Code::Blocks Integrated Version

This project is developed for Assignment 01 of the 11302_AI_Concept course.
It implements three classic AI search algorithms: DFS, BFS, and A*, and includes a result verifier to validate solutions for 3SAT problems.

*Project Structure*

- main.cpp – Main interface (menu-driven)
- DFS.cpp – Depth-First Search algorithm
- BFS.cpp – Breadth-First Search algorithm
- Astar.cpp – A* Search algorithm
- Verify.cpp – Validates the result output
- common.h / common.cpp – Shared utilities and global variables
- /data/ – Contains input CSV files with 3SAT clauses
- /results/ – Output folder for generated solution files

*Requirements*

Code::Blocks IDE (with MinGW or any C++17-compliant compiler)
C++17 or later standard enabled
No external libraries needed

*How to Use (via Code::Blocks)*

1. Open AI_Assignment01.cbp in Code::Blocks.
2. Press F9 to build and run the project.
3. You will see a menu like:

   ==== 3SAT Solver ====
   Choose algorithm:
   1. DFS
   2. BFS
   3. A* Search
   4. Verify result file
   Enter a number to select your algorithm.

4. When prompted, input the file path to a CSV file like:
      ./data/3SAT_Dim=10.csv
5. Output will be saved under the /results/ folder, e.g.,
      ./results/DFS/Result_Dim=10.txt

*Notes*

- If you press Enter without typing a path, it uses the default path:
    ./data/3SAT_Dim=10.csv
- The Verifier checks whether the vector view in the result file satisfies all the original 3SAT clauses.
- Ensure the CSV files follow the correct format (each clause with three literals, comma-separated).
- To add new instances, place them under the /data/ folder with filenames like 3SAT_Dim=20.csv.
