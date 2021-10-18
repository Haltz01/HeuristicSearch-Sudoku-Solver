#ifndef SUDOKU_SOLVER
#define SUDOKU_SOLVER

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <queue>
#include <numeric>
#include <bitset>
#include <ctime>

class SudokuSolver {
    private:
        int boardSize; // 9 by 9 board is the most commom one -> boardSize = 9
        int blockSize; // usually, blocks are 3 by 3 in a 9x9 board -> blockSize = 3
        std::vector<int> usedValuesRow; // already used values (can't be assigned to cells) for each row in the board
        std::vector<int> usedValuesColumn; // already used values (can't be assigned to cells) for each column in the board
        std::vector<int> usedValuesBlock; // already used values (can't be assigned to cells) for each block in the board
        std::vector<std::vector<int>> board; // sudoku board -> empty cells are marked as 0
        std::vector<std::vector<int>> nbOfValuesAvailableToCell; // contains the amount of values that can be inserted at each cell
        std::priority_queue<std::pair<int, std::pair<int, int>>> cellsToVisit; // priority_queue used to determine which will be the next visited cell, this cell will be assigned a value -> the "pair<pair>" is this (nbUsedValuesForCell, (cell_line, cell_column))
        std::vector<std::vector<bool>> visited;
    public:
        SudokuSolver(std::vector<std::vector<int>> & sudokuBoard);
        SudokuSolver(std::vector<std::vector<int>> & sudokuBoard, int boardSize, int blockSize); // constructor
        std::vector<int> getAvailableValuesForCell(int row, int col); // returns an array with all the available values that can be assgned to a cell
        void heuristicSearch();
        bool backtrackingSearch(std::vector<std::vector<bool>> & visited);
        void initialValuesLookup();
        void solve();
        void printBoard();
        void copyBoard(std::vector<std::vector<int>> & boardToCopy);
        void initialAdding();
        bool valueCausesDuplicates(int curCellLine,int curCellCol,int valueToTry);
        bool checkIfValueRepeats(int row, int col,int checkValue);
        void copyPriorityQueue(std::priority_queue<std::pair<int, std::pair<int, int>>> & priorityQueueBackup);

};

#endif