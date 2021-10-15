#include "solver.hpp"

SudokuSolver::SudokuSolver(int board_size, int block_size) {
    if (blockSize*blockSize != boardSize) {
        printf("[!] ERROR: invalid board_size/block_size\board_size must be equal to block_size² !\n");
        return;
    }

    boardSize = board_size; 
    blockSize = block_size;
    availableValuesRow = std::vector<std::set<int>>(boardSize);
    availableValuesColumn = std::vector<std::set<int>>(boardSize);
    availableValuesBlock = std::vector<std::set<int>>(boardSize);
    board = std::vector<std::vector<int>>(boardSize);
}

std::vector<int> SudokuSolver::getAvailableValueForCell(int row, int col) {
    int blockIndex = 0;
    blockIndex += blockSize * floor((row/blockSize));
    blockIndex += floor((col/blockSize));

    std::vector<int> valueFreq(10, 0); // counts the frequencies of 1...9 to see which of them apper in all three sets declared in constructor

    for (int x : availableValuesRow[row])
        valueFreq[x]++;

    for (int x : availableValuesColumn[col])
        valueFreq[x]++;

    for (int x : availableValuesBlock[blockIndex])
        valueFreq[x]++;

    std::vector<int> availableValues;

    for (int i = 1; i <= 9; i++) {
        if (valueFreq[i] == 3) // if the count is 3, the value appers in all sets and can be inserted in cell
            availableValues.push_back(i);
    }

    return availableValues;
}
