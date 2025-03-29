AI_Assignment01

This project is for Assignment 01 of the 11302_AI_Concept course. It implements three classical AI search algorithms in C++: DFS, BFS, and A*.

Project Contents:
- DFS.cpp
- BFS.cpp
- Astar.cpp
- /data folder containing input CSV files
- /results folder where output will be saved

Requirements:
- C++17 or higher
- g++ or any C++ compiler that supports C++17

Execution Steps:
1. Download and unzip `AI_Assignment01.zip`.
2. Open a terminal and compile the programs:
   g++ DFS.cpp -o DFS  
   g++ BFS.cpp -o BFS  
   g++ Astar.cpp -o Astar
3. Run the executable of the algorithm you want to test:
   ./DFS.exe
   ./BFS.exe
   ./Astar.exe
4. When prompted, enter the path to the input file. Example:
   ./data/3SAT_Dim=10.csv
5. The result will be saved to the `./results` folder.
6. If no solution is found, the console will show:
   No answer found

Notes:
- Make sure the input file exists in the correct path.
- On Windows, use `DFS.exe`, `BFS.exe`, or `Astar.exe` to run.
