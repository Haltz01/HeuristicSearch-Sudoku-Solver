#ifndef SUDOKU_SOLVER
#define SUDOKU_SOLVER

#include <iostream>
#include <vector>
#include <set>
#include <cmath>

class SudokuSolver {
    private:
        int boardSize; // 9 by 9 board is the most commom one -> boardSize = 9
        int blockSize; // usually, blocks are 3 by 3 in a 9x9 board -> blockSize = 3
        std::vector<std::set<int>> availableValuesRow; // available values (can be assigned to cells) for each row in the board
        std::vector<std::set<int>> availableValuesColumn; // available values (can be assigned to cells) for each column in the board
        std::vector<std::set<int>> availableValuesBlock; // available values (can be assigned to cells) for each block in the board
        std::vector<std::vector<int>> board; // sudoku board -> empty cells are marked as 0

    public:
        SudokuSolver(int boardSize, int blockSize); // constructor
        std::vector<int> getAvailableValueForCell(int row, int col); // returns an array with all the available values that can be assgned to a cell
};

#endif