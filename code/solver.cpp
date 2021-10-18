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
    //copyBoard(sudokuBoard);
    nbOfValuesAvailableToCell = std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0));
}

/*
void SudokuSolver::copyBoard(std::vector<std::vector<int>> & boardToCopy){
    board = std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0));
    std::cout << "copyBoard" << std::endl;
    std::cout << "board size = " << board.size() << std::endl;
    for(int i = 0; i < boardSize; i++){
        for(int j = 0; j < boardSize; j++){
            std::cout << boardToCopy.at(i).at(j) << " ";
            //board.at(i).at(j) = boardToCopy.at(i).at(j);
        }
        std::cout << std::endl;
    }
}
*/
//e
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
            if (board[i][j] == 0)
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
    std::cout << std::endl;
    for(int val : availableValues){
        std::cout << val << " ";
    }
    std::cout << std::endl;
    return availableValues;
}




bool SudokuSolver::checkIfValueRepeats(int row, int col,int checkValue) {
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
        if(curValue == checkValue && valueFreq[curValue] != 3) return true;
    }
    return false;
}







bool SudokuSolver::valueCausesDuplicates(int curCellLine,int curCellCol,int valueToTry){
    int blockIndex = blockSize * floor((curCellLine/blockSize)) + floor((curCellCol/blockSize));
    int lineBlock,colBlock;
    std::vector<int> availableValues;
    for(int i =0;i<boardSize;i++){
        if (board[curCellLine][i] == 0 && checkIfValueRepeats(curCellLine,i,valueToTry)) { // LINE
            std::cout << "error on (" << curCellLine << "," << i << ")\n";
            return true;
        }

        if (board[i][curCellCol] == 0 && checkIfValueRepeats(i,curCellCol,valueToTry)) { // COLUMN
            std::cout << "error on (" << i << "," << curCellCol << ")\n";
            return true;
        }

        lineBlock = blockSize*(blockIndex/blockSize) + i/blockSize;
        colBlock = blockSize*(blockIndex%blockSize) + i%blockSize;
        if (board[lineBlock][colBlock] == 0 && checkIfValueRepeats(lineBlock,colBlock,valueToTry)) { // BLOCK
            std::cout << "error on (" << lineBlock << "," << colBlock << ")\n";
            return true;
        }  
    }
    return false;
}


bool SudokuSolver::backtrackingSearch(std::vector<std::vector<bool>> & visited) {
    bool isBoardCompleted = true;
    bool toBreak = false;
    std::cout << "Entering backtracking" << std::endl;

    printBoard();

    //std::vector<std::vector<bool>> 
    //TODO: Backup of visited, check if returning does not break the program

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

    if (isBoardCompleted){
        std::cout << "@Board is completed" << std::endl;
        return true;
    } // values are 0, so the board was 100% filled


    if(cellsToVisit.empty()){
        std::cout << "$No cells to visit" << std::endl;
        return false;
    }
    std::cout << "Number of cells to visit = " << cellsToVisit.size() << std::endl;
    
    // Visiting cell with max priority (has less options of values to be assigned)
    int curCellLine = cellsToVisit.top().second.first; // represents the cell row index
    int curCellCol = cellsToVisit.top().second.second;// represents the cell column index
    int nbAvailableValues = boardSize - cellsToVisit.top().first; // number of used values, so the available values is boardSize - usedValues
    cellsToVisit.pop();
    std::cout << "\n===== VISITING NEXT  CELL =====\n";
    std::cout << "line = " << curCellLine << " ";
    std::cout << "; col = " << curCellCol << " ";
    std::cout << "; N.A.V. = " << nbAvailableValues << std::endl;

    if (visited[curCellLine][curCellCol]){
        std::cout << "$ Cell already visited, returning...\n";
        return false;
    }
    // mark current cell as visited
    visited[curCellLine][curCellCol] = true;

    // backup of priority_queue to do the "rewind" in the backtrack
    std::priority_queue<std::pair<int, std::pair<int, int>>> cellsToVisit_BACKUP = cellsToVisit;

    // get all possible values to assign to cell
    std::vector<int> availableValues = getAvailableValuesForCell(curCellLine, curCellCol);
    std::cout << "Available values to insert in cell =\n";
    for(int val : availableValues){
        std::cout << "\t" << val << std::endl;
    }
    // propagate information of value assignment to all other empty cells in same row, column and block
    std::set<std::pair<int, int>> cellsToInsertInPq;

    int blockIndex = blockSize * floor((curCellLine/blockSize)) + floor((curCellCol/blockSize));
    int lineBlock,colBlock;
    std::vector<std::vector<int>> nbOfValuesAvailableToCell_BACKUP;

    for (int valueToTry : availableValues) { // trying each value possible to current cell
        std::cout << "Trying value " << valueToTry << " in cell " << curCellLine << ", " << curCellCol << std::endl;
        nbOfValuesAvailableToCell_BACKUP = nbOfValuesAvailableToCell;

        for (int i = 0; i < boardSize; i++) {
            if (curCellCol != i && board[curCellLine][i] == 0 && !checkIfValueRepeats(curCellLine,i,valueToTry)) { // LINE
                if(nbOfValuesAvailableToCell[curCellLine][i] == 1){
                    std::cout << "$ Cell [" << curCellLine << "][" << i << "] can't have a value assigned to -> Returning..." << std::endl;
                    nbOfValuesAvailableToCell = nbOfValuesAvailableToCell_BACKUP;
                    // resets priority_queue
                    cellsToVisit = cellsToVisit_BACKUP;
                    // backtracking after DFS
                    visited[curCellLine][curCellCol] = false;
                    return false;
                }
                nbOfValuesAvailableToCell[curCellLine][i]--;
                if (!visited[curCellLine][i])
                    cellsToInsertInPq.insert(std::make_pair(curCellLine, i));
            }

            if (curCellLine != i && board[i][curCellCol] == 0 && !checkIfValueRepeats(i,curCellCol,valueToTry)) { // COLUMN
                if(nbOfValuesAvailableToCell[i][curCellCol] == 1){
                    std::cout << "$ Cell [" << i << "][" << curCellCol << "] can't have a value assigned to -> Returning..." << std::endl;
                    nbOfValuesAvailableToCell = nbOfValuesAvailableToCell_BACKUP;
                    // resets priority_queue
                    cellsToVisit = cellsToVisit_BACKUP;
                    // backtracking after DFS
                    visited[curCellLine][curCellCol] = false;
                    return false;
                }
                nbOfValuesAvailableToCell[i][curCellCol]--;
                if (!visited[i][curCellCol])
                    cellsToInsertInPq.insert(std::make_pair(i, curCellCol));
            }

            lineBlock = blockSize*(blockIndex/blockSize) + i/blockSize;
            colBlock = blockSize*(blockIndex%blockSize) + i%blockSize;
            if(lineBlock == curCellLine || colBlock == curCellCol)
                continue;
            if (board[lineBlock][colBlock] == 0 && !checkIfValueRepeats(lineBlock,colBlock,valueToTry)) { // BLOCK
                if(nbOfValuesAvailableToCell[lineBlock][colBlock] == 1){
                    std::cout << "$ Cell [" << lineBlock << "][" << colBlock << "] can't have a value assigned to -> Returning..." << std::endl;
                    nbOfValuesAvailableToCell = nbOfValuesAvailableToCell_BACKUP;
                    // resets priority_queue
                    cellsToVisit = cellsToVisit_BACKUP;
                    // backtracking after DFS
                    visited[curCellLine][curCellCol] = false;
                    return false;
                } 
                nbOfValuesAvailableToCell[lineBlock][colBlock]--;
                if (!visited[lineBlock][colBlock])
                    cellsToInsertInPq.insert(std::make_pair(lineBlock, colBlock));
            }  

        }

        // add cells to priority queue with new priority values
        for (std::pair<int, int> insertCell : cellsToInsertInPq) {
            int row = insertCell.first;
            int col = insertCell.second;
            //if(nbOfValuesAvailableToCell[row][col] == 0) return false;
            std::cout << "\t New cell in PQ: (" << row << ", " << col << ") N.A.V = " << nbOfValuesAvailableToCell[row][col] << "\n";
            cellsToVisit.push({boardSize - nbOfValuesAvailableToCell[row][col], {row, col}});
        }

        // decide the value to assign to current cell
        board[curCellLine][curCellCol] = valueToTry;

        // update values for usedValuesRow, usedValuesCol, usedValuesBlock
        usedValuesRow[curCellLine] += std::pow(2, valueToTry-1);
        usedValuesColumn[curCellCol] += std::pow(2, valueToTry-1);
        usedValuesBlock[blockIndex] += std::pow(2, valueToTry-1); 
        
        // go deeper in our decision tree (board states), visiting another tree -> DFS
        isBoardCompleted = backtrackingSearch(visited);
        std::cout << "Returning from backing track from (" << curCellLine << ", " << curCellCol << ") was with value " << valueToTry  << std::endl;  
        if(isBoardCompleted) // if we completed the board, the challenge is over, we can stop the search
            return true;

        // undo updates in usedValuesRow, usedValuesCol, usedValuesBlock
        usedValuesRow[curCellLine] -= std::pow(2, valueToTry-1);
        usedValuesColumn[curCellCol] -= std::pow(2, valueToTry-1);
        usedValuesBlock[blockIndex] -= std::pow(2, valueToTry-1);
        board[curCellLine][curCellCol] = 0;

        nbOfValuesAvailableToCell = nbOfValuesAvailableToCell_BACKUP;
    }

    // starting "backtracking rewind"...

    // if (!isBoardCompleted)... (if we reach here, the board won't be complete)
    // ...undo propagation of value assignment to all other empty cells in same row, column and bloc
    
    // resets priority_queue
    cellsToVisit = cellsToVisit_BACKUP;

    // backtracking after DFS
    visited[curCellLine][curCellCol] = false;

    return false;
}

void SudokuSolver::printBoard(){
    std::cout << "board size = " << board.size() << std::endl;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if(j%blockSize == 0) std::cout << "| ";
            std::cout << board[i][j] << " ";
        }
        if((i+1)%blockSize == 0) std::cout << "|\n|-----------------------|\n";
        else std::cout << "|\n";
    }
}

void SudokuSolver::initialAdding(){
    int availableQtty;
    for(int i = 0;i<boardSize;i++){
        for(int j = 0;j<boardSize;j++){
            if(board[i][j] == 0){
                availableQtty = getAvailableValuesForCell(i,j).size();
                std::cout << "line = " << i << "; col = " << j << "; N.A.V. = " << availableQtty << std::endl;
                cellsToVisit.push({boardSize - availableQtty,{i,j}});
            }
        }
    }
}


void SudokuSolver::heuristicSearch() {
    //std::vector<std::vector<bool>> visited(boardSize, std::vector<bool>(boardSize, false));
    visited = std::vector<std::vector<bool>>(boardSize, std::vector<bool>(boardSize, false));
    initialValuesLookup();
    initialAdding();
    bool sudokuSolved = false;

    sudokuSolved = backtrackingSearch(visited);
    if(sudokuSolved){
        std::cout << "We've got an answer!\n";
    }
    else{
        std::cout << "Not this time :(\n";
    }
    printBoard();

    return;
}

void SudokuSolver::solve() {
    heuristicSearch();
}
