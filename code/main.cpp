#include "solver.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
void splitString(std::string const &str, const char delim, std::vector<int> &out) {   
    std::stringstream sstream(str);  
    std::string s2;
    while (std::getline (sstream, s2, delim)) {
        out.push_back(std::stoi(s2));
    }
    
    return;
}

std::vector<std::vector<int>> readFile(std::string filePath){
    std::string lineText;
    std::vector<int> splittedText;
    std::vector<std::vector<int>> sudokuBoard;
    std::ifstream myFile(filePath);

    while(getline(myFile,lineText)){
        splittedText = std::vector<int>();
        splitString(lineText, ' ', splittedText);
        sudokuBoard.push_back(splittedText);
    }
    // for(std::vector<int> line : sudokuBoard){
    //     for(int c: line) std::cout << c << " ";
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    return sudokuBoard;
}



int main(int argc, char const *argv[]) {
    // Reads sudoku board file
    std::vector<std::vector<int>> sudokuBoard = readFile(std::string(argv[1])); 

    // When we accept boards with different sizes, this should be checked:
    // if (blockSize*blockSize != boardSize) {
    //     printf("[!] ERROR: invalid board_size/block_size\board_size must be equal to block_size² !\n");
    //     return;
    // }
    SudokuSolver solver = SudokuSolver(sudokuBoard);
    solver.solve();

    return 0;
}
