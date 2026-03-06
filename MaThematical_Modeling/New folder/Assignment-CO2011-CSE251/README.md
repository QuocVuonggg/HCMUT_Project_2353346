========================================================================
PROJECT: SYMBOLIC AND ALGEBRAIC REASONING IN PETRI NETS
Course: Mathematical Modeling (CO2011) - Semester 1 (2025-2026)
University: HCMC University of Technology (HCMUT)
========================================================================

1. INTRODUCTION
------------------------------------------------------------------------
This software implements algorithms for analyzing 1-safe Petri nets using:
- Explicit State Enumeration (BFS)
- Symbolic Model Checking (Binary Decision Diagrams - BDD)
- Integer Linear Programming (ILP)

It solves Reachability, Deadlock Detection, and Reachability Optimization.

2. GROUP MEMBERS
------------------------------------------------------------------------
ID: 2353346 - Name: Phạm Quốc Vương (Leader) - Task 3
ID: 2352227 - Name: Hồ Trần Minh Đạt - Task 4
ID: 2352327 - Name: Nguyễn Hiếu - Task 2
ID: 2452514 - Name: Nguyễn Trần Tuấn Khải - Task 1
ID: 2353345 - Name: Nguyễn Viết Tấn Vương - Task 5

3. PROJECT STRUCTURE
------------------------------------------------------------------------
Assignment-CO2011-CSE251-2352227-2352327-2452514-2353345-2353346/   <-- [ZIP FOLDER]
|
|-- Report.pdf                                                      <-- Project Report file
|
|-- Assignment-CO2011-CSE251/                                       <-- [ASSIGNMENT FOLDER]
    |
    |-- README.md                          # This file
    |
    |-- src/                                # Source code directory
    |   |-- main.py                         # Main execution file
    |   |-- petri_net.py                    # Petri Net class definition
    |   |-- pnml_parser.py                  # Task 1: PNML Parser
    |   |-- explicit_reachability.py        # Task 2: Explicit BFS
    |   |-- symbolic_reachability.py        # Task 3: BDD Reachability
    |   |-- deadlock_detection.py           # Task 4: ILP + BDD Deadlock
    |   |-- reachability_optimization.py    # Task 5: Optimization
    |
    |-- tc/                                 # Test cases (PNML files)
        |-- tc_all.pnml
        |-- tc_large.pnml
        |-- tc1.pnml
        |-- tc2.pnml
        |-- tc3.pnml
        |-- tc4.pnml
        |-- tc5.pnml

4. INSTALLATION & SETUP
------------------------------------------------------------------------
This project requires Python 3. The following libraries must be installed:
- dd (for BDDs)
- pulp (for ILP)

STEP 1: Create a virtual environment (execute at project root)
   python -m venv .venv

STEP 2: Activate the environment
    .venv\Scripts\Activate.ps1

STEP 3: Install dependencies
   pip install dd 
   pip install pulp

5. USAGE
------------------------------------------------------------------------
Run the program from the project root directory using `src/main.py`.

Syntax:
   python src/main.py --task <TASK_ID> --file <FILENAME>

Arguments:
   --task : 1, 2, 3, 4, 5, or all
   --file : Name of the PNML file inside the 'tc' folder

EXAMPLES:

(1) Parse PNML file and show statistics:
    python src/main.py --task 1 --file tc1.pnml

(2) Run Explicit Reachability (BFS):
    python src/main.py --task 2 --file tc1.pnml

(3) Run Symbolic Reachability (BDD):
    python src/main.py --task 3 --file tc1.pnml

(4) Detect Deadlock (using ILP + BDD):
    python src/main.py --task 4 --file deadlock_trap.pnml

(5) Optimize Objective Function over Reachable Set:
    python src/main.py --task 5 --file tc1.pnml

(6) Run ALL tasks for a comprehensive report:
    python src/main.py --task all --file tc1.pnml

6. TASK DESCRIPTIONS
------------------------------------------------------------------------
- Task 1: Parses PNML files to build the internal Petri Net model.
- Task 2: Computes reachable markings using explicit Breadth-First Search.
- Task 3: Computes reachable markings symbolically using BDDs (dd library).
- Task 4: Detects deadlocks by combining structural analysis (ILP) with 
          reachability analysis (BDD) to avoid spurious deadlocks.
- Task 5: Finds a reachable marking that maximizes a weighted token sum 
          using ILP constraints on the state equation and reachability.

7. NOTE
------------------------------------------------------------------------
- Ensure the 'tc' folder contains the valid PNML files before running.
- Performance depends on the complexity of the net (state explosion).
- Task 3 relies on the 'dd' library.
- Task 4 & 5 rely on the 'pulp' solver.