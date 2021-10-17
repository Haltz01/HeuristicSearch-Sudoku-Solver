#include "solver.hpp"
SudokuSolver::SudokuSolver(std::vector<std::vector<int>> & sudokuBoard) :
    SudokuSolver(sudokuBoard, 9, 3){}

SudokuSolver::SudokuSolver(std::vector<std::vector<int>> & sudokuBoard, int board_size, int block_size) {
    boardSize = board_size;
    blockSize = block_size;
    int initValue = 0; // all values were not used, so all bits must be 0

    usedValuesRow = std::vector<int>(boardSize, initValue);
    usedValuesColumn = std::vector<int>(boardSize, initValue);
    usedValuesBlock = std::vector<int>(boardSize, initValue);
    board = sudokuBoard;
    nbOfValuesAvailableToCell = std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0));
}

void SudokuSolver::initialValuesLookup() {
    int blockIndex;
    int lineBlock,colBlock;

    // iterate thought each cell to fill usedValuesRow, usedValuesColumn, usedValuesBlock
    for (int i = 0; i < boardSize; i++) { 
        for (int j = 0; j < boardSize; j++) {
            blockIndex = blockSize * floor((i/blockSize)) + floor((j/blockSize));

            if (board[i][j] != 0) {
                usedValuesRow[i] += std::pow(2, board[i][j]-1);
                usedValuesColumn[j] += std::pow(2, board[i][j]-1);
                usedValuesBlock[blockIndex] += std::pow(2, board[i][j]-1);
            }
        }
    }

    for (int i = 0; i < boardSize; i++) { 
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] != 0)
                nbOfValuesAvailableToCell[i][j] = (int)getAvailableValuesForCell(i, j).size();
        }
    }

    return;
}


//TODO: comment how availableValues work
std::vector<int> SudokuSolver::getAvailableValuesForCell(int row, int col) {
    int blockIndex = 0;
    blockIndex += blockSize * floor((row/blockSize));
    blockIndex += floor((col/blockSize));
    int curValue;
    std::vector<int> valueFreq(10, 0); // counts the frequencies of 1...9 to see which of them apper in all three sets declared in constructor
    std::vector<int> availableValues;

    for (int i = 0; i < boardSize; i++) {
        //For each of the attributes - row, column and block - we verify if the value is in the avaulableValues. 
        curValue = i+1;
        valueFreq[curValue] += ((int)std::pow(2,i) & usedValuesRow[row]) ? 0 : 1;
        valueFreq[curValue] += ((int)std::pow(2,i) & usedValuesColumn[col]) ? 0 : 1;
        valueFreq[curValue] += ((int)std::pow(2,i) & usedValuesBlock[blockIndex]) ? 0 : 1;
        if(valueFreq[curValue] == 3) availableValues.push_back(curValue);
    }
    
    return availableValues;
}

bool SudokuSolver::backtrackingSearch(std::vector<std::vector<bool>> & visited) {
    bool isBoardCompleted = true;
    bool toBreak = false;

    // Checking if we have a completed board
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == 0) {
                isBoardCompleted = false;
                toBreak = true;
                break;
            }
            if(toBreak) break;
        }
    }

    if (isBoardCompleted) // values are 0, so the board was 100% filled
        return true;

    if (cellsToVisit.empty())
        return false;
    
    // Visiting cell with max priority (has less options of values to be assigned)
    int curCellLine = cellsToVisit.top().second.first; // represents the cell row index
    int curCellCol = cellsToVisit.top().second.second;// represents the cell column index
    int nbAvailableValues = boardSize - cellsToVisit.top().first; // number of used values, so the available values is boardSize - usedValues
    cellsToVisit.pop();

    if (visited[curCellLine][curCellCol])
        return false;

    // mark current cell as visited 
    visited[curCellLine][curCellCol] = true;

    // backup of priority_queue to do the "rewind" in the backtrack
    std::priority_queue<std::pair<int, std::pair<int, int>>> cellsToVisit_BACKUP = cellsToVisit;

    // get all possible values to assign to cell
    std::vector<int> availableValues = getAvailableValuesForCell(curCellLine, curCellCol);

    // propagate information of value assignment to all other empty cells in same row, column and block
    std::set<std::pair<int, int>> cellsToInsertInPq;

    int blockIndex = blockSize * floor((curCellLine/blockSize)) + floor((curCellCol/blockSize));
    int lineBlock,colBlock;
    for (int i = 0; i < boardSize; i++) {
        if (board[curCellLine][i] == 0) { // LINE
            nbOfValuesAvailableToCell[curCellLine][i]--;
            if (!visited[curCellLine][curCellCol])
                cellsToInsertInPq.insert(std::make_pair(curCellLine, i));
        }

        if (board[i][curCellCol] == 0) { // COLUMN
            nbOfValuesAvailableToCell[i][curCellCol]--;
            if (!visited[curCellLine][curCellCol])
                cellsToInsertInPq.insert(std::make_pair(i, curCellCol));
        }

        lineBlock = blockSize*(blockIndex/blockSize) + i/blockSize;
        colBlock = blockSize*(blockIndex%blockSize) + i%blockSize;
        if (board[lineBlock][colBlock] == 0) { // BLOCK
            nbOfValuesAvailableToCell[lineBlock][colBlock]--;
            if (!visited[curCellLine][curCellCol])
                cellsToInsertInPq.insert(std::make_pair(lineBlock, colBlock));
        }  

    }

    // add cells to priority queue with new priority values
    for (std::pair<int, int> insertCell : cellsToInsertInPq) {
        int row = insertCell.first;
        int col = insertCell.second;
        cellsToVisit.push({nbOfValuesAvailableToCell[row][col], {row, col}});
    }

    for (int valueToTry : availableValues) {
        // decide the value to assign to current cell
        board[curCellLine][curCellCol] = valueToTry;

        // update values for usedValuesRow, usedValuesCol, usedValuesBlock
        usedValuesRow[curCellLine] += std::pow(2, valueToTry-1);
        usedValuesColumn[curCellCol] += std::pow(2, valueToTry-1);
        usedValuesBlock[blockIndex] += std::pow(2, valueToTry-1); 
        
        // go deeper in our decision tree (board states), visiting another tree -> DFS
        isBoardCompleted = backtrackingSearch(visited);
        if(isBoardCompleted) // if we completed the board, the challenge is over, we can stop the search
            return true;
        // undo updates in usedValuesRow, usedValuesCol, usedValuesBlock
        usedValuesRow[curCellLine] -= std::pow(2, valueToTry-1);
        usedValuesColumn[curCellCol] -= std::pow(2, valueToTry-1);
        usedValuesBlock[blockIndex] -= std::pow(2, valueToTry-1); 
    }

    // starting "backtracking rewind"...

    // if (!isBoardCompleted)... (if we reach here, the board won't be complete)
    // ...undo propagation of value assignment to all other empty cells in same row, column and bloc
    for (int i = 0; i < boardSize; i++) {
        if (board[curCellLine][i] == 0) { // LINE
            nbOfValuesAvailableToCell[curCellLine][i]++;
        }

        if (board[i][curCellCol] == 0) { // COLUMN
            nbOfValuesAvailableToCell[i][curCellCol]++;
        }

        lineBlock = blockSize*(blockIndex/blockSize) + i/blockSize;
        colBlock = blockSize*(blockIndex%blockSize) + i%blockSize;
        if (board[lineBlock][colBlock] == 0) { // BLOCK
            nbOfValuesAvailableToCell[lineBlock][colBlock]++;
        }
    }

    // resets priority_queue
    cellsToVisit = cellsToVisit_BACKUP;

    // backtracking after DFS
    visited[curCellLine][curCellCol] = false;

    // refill priority_queue...
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == 0 && !visited[i][j]) {
                cellsToVisit.push({nbOfValuesAvailableToCell[i][j], {i, j}});
            }
        }
    }

    return false;
}

void SudokuSolver::heuristicSearch() {
    std::vector<std::vector<bool>> visited(blockSize, std::vector<bool>(blockSize, false));
 
    backtrackingSearch(visited);
    
    return;
}
