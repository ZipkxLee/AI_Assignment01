** AI Assignment01 - Solving 3-SAT with DFS / BFS / A* Search **

This project is the assignment for the "Introduction to Artificial Intelligence" course at Fu Jen Catholic University. The goal is to use three classic search algorithms (DFS, BFS, and A*) to solve the Boolean 3-SAT problem and compare their performance.

*Problem Definition*

The 3-SAT problem is a specific case of the Boolean satisfiability problem (SAT), where the goal is to find a Boolean assignment for a set of variables that satisfies all the clauses in a 3-SAT formula. In this assignment, the objective is to find an assignment of variables that satisfies all the clauses using DFS, BFS, and A* search algorithms. The performance of each algorithm is compared based on the total number of clause checks, expanded nodes, and running time.

*Folder Structure*

Assignment01/

├── datas/             # Contains test data (CSV format)

│   └── 3SAT_Dim=30.csv

├── results/           # Contains the output of search results (automatically generated)

│   ├── DFS/

│   ├── BFS/

│   └── Astar/

├── common.h

├── dfs.h

├── bfs.h

├── astar.h

├── main.cpp

└── README.md

*Compilation Requirements*

- C++17 or higher
- Compiler that supports `<filesystem>` header (e.g., GCC 9+, Clang 10+, MSVC 2019+)

*Recommended Compilation Command:*

g++ sources/main.cpp -I Header -std=c++17 -o sat_solver

*Usage Instructions*

1. Place the data files in the `./datas/` folder. The file name should include the `Dim=XX` format (e.g., `3SAT_Dim=30.csv`).
2. Run the main program:

./sat_solver

3. The program will prompt for the algorithm selection:

Select the algorithm:
1: DFS
2: BFS
3: A*
4: Exit
Enter your choice: 1

Enter the data file path (default: ./datas/3SAT_Dim=30.csv):

4. The results will be automatically saved in the corresponding folder within `results/` (e.g., `results/DFS/`).

Input Format

- Each `.csv` file contains the clauses of the 3-SAT problem. The first line contains `Dim=XX` (where XX is the number of variables).
- Each subsequent line contains a clause represented by three literals (e.g., `1,-2,3` means the clause `(x1 OR ¬x2 OR x3)`).
  
Example:

1,-3,4

-2,-4,5

3,-1,2

Output Format

The result will be saved in the corresponding folder within `results/` as `result_Dim=XX.txt`. The format is as follows:

-- Answer Found:

	x1: False

	x2: True

	...

	x30: True


-- Vector View:

	{ 0 1 0 1 1 ... }

-- Total Clause Cost:

	12345678

-- Expanded Nodes:

	314159

-- Running Time:

	987 ms

Special Features

- Automatically detects the `Dim=` value from the file name.
- Clears the screen after each search to provide a clean interface.
- Automatically creates the result output directory.
- Combines static and dynamic variable ordering strategies (for BFS and A*).

*Compilation and Execution*

1. Compile the Code
Ensure your compiler supports C++17 or higher and has the `<filesystem>` header.

g++ sources/main.cpp -I Header -std=c++17 -o sat_solver

2. Run the Program
After compilation, run the program:

./sat_solver

3. Algorithm Selection
The program will ask you to select the algorithm:

Select the algorithm:
1: DFS

2: BFS

3: A*

4: Exit
Enter your choice:

After entering your choice, the program will prompt for the data file path. If no path is provided, it will default to `./datas/3SAT_Dim=30.csv`.

4. Output Files
The program will output results in the `results/` folder:

- DFS results: `results/DFS/result_Dim=XX.txt`
- BFS results: `results/BFS/result_Dim=XX.txt`
- A* results: `results/Astar/result_Dim=XX.txt`

*Input Data Format*

The input data must be a CSV file with each subsequent line represents a clause, containing three literals (each integer represents a literal: positive for true, negative for false).

Example CSV file (3SAT_Dim=30.csv):

1,-3,4

-2,-4,5

3,-1,2

*Output Data Format*

Each algorithm's output will include the following:

- **Answer Found**: Indicates if a solution was found, followed by the variable assignments.
- **Vector View**: Displays the solution vector, with each variable assigned to either True or False.
- **Total Clause Cost**: The number of clause checks performed during the search.
- **Expanded Nodes**: The number of nodes explored during the search.
- **Running Time**: The time taken by the algorithm to run, measured in milliseconds.

Example output:

-- Answer Found:
	x1: False
	x2: True
	...
	x30: True

-- Vector View:
	{ 0 1 0 1 1 ... }

-- Total Clause Cost:
	12345678

-- Expanded Nodes:
	314159

-- Running Time:
	987 ms

*Conclusion*

This project compares three different search algorithms (DFS, BFS, and A*) to solve the 3-SAT problem. The results demonstrate how each algorithm performs in terms of expanded nodes, clause checks, and running time across different problem dimensions.

*Future Improvements:*
- Enhance the heuristic function for A* to improve its efficiency.
- Combine dynamic and static variable ordering strategies for better performance on large instances.
- Improve A* performance by implementing an effective heuristic.
