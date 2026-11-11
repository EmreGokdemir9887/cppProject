#ifndef SetSolver_h
#define SetSolver_h

#include <string>
#include <iostream>
#include <vector>
#include "SetSolverSquareSet.h"

using std::vector;
using std::string;
//You can add another .h class to help you create a data structure of your choosing.

class SetSolver
{
private:
    int boardSize = 9;
    vector<vector<int>> compartments;
    vector<vector<SetSolverSquareSet>>board;

bool hasCandidateBetween(int row, int column, int low, int high) {
    const SetSolverSquareSet & square = board[row][column];
    for(int candidate : square.set) {
        if(candidate>= low && candidate <= high) {
            return true;
        }
    }
    return false;
}

bool runFits(const vector<int> & compartment, int start) {
    int length = (int)compartment.size();
    int end = start + length - 1;
    for(int location : compartment) {
        int row = location / 9;
        int column = location % 9;
        if(!hasCandidateBetween(row, column, start, end)) {
            return false;
        }
    }
    return true;
}

bool applyRule2ToCompartment(const vector<int> & compartment, bool & isChanged) {
    int length = (int)compartment.size();
    bool allowed[10] = {false};
    bool anyRunFits = false;
    for(int start = 1; start <= (10 - length); ++start) {
        if(runFits(compartment, start)) {
            anyRunFits = true;
            for(int d = start; d <= start + length - 1; ++d) {
                allowed[d] = true;
            }
        }
    }
    if(!anyRunFits) {return false;}
    for(int location : compartment) {
        int row = location / 9;
        int column = location % 9;
        for(int d = 1; d < 10; ++d) {
            if(allowed[d] == false) {
                if(removeFromCell(row, column, d, isChanged) == false) {return false;}
            }
        }
    }
    return true;
}

bool applyRule2(bool & isChanged) {
    for(const auto & compartment : compartments) {
        if(applyRule2ToCompartment(compartment, isChanged) == false) {
            return false;
        }
    }
    return true;
}

int knownDigit(int row, int column) {
    const SetSolverSquareSet & square = board[row][column];
    if(!square.white) {
        return (-square.value);
    }
    else if(square.set.size() == 1) {
        return square.set[0];
    }
    else {
        return 0;
    }
}

bool removeFromCell(int row, int column, int data, bool & isChanged) {
    SetSolverSquareSet & square = board[row][column];
    if(!square.white) {
        return true;
    }
    if(square.remove(data)) {
        isChanged = true;
        if(square.set.empty()) {
            return false;
        }
    }
    return true;
}

bool applyRule1(bool & isChanged) {
    for(int row = 0; row < 9; ++row) {
        for(int column = 0; column < 9; ++column) {
            int data = knownDigit(row, column);
            if(data == 0) {continue;}

            for(int k = 0; k < 9; ++k) {
                if(k != column) {
                    if(removeFromCell(row, k, data, isChanged) == false) {
                        return false;
                    }
                }
                if(k != row) {
                    if(removeFromCell(k, column, data, isChanged) == false) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void findCompartmentsRow() {
    vector<int> current;
    for(int rowCount = 0; rowCount < 9; ++rowCount) {
        for(int columnCount = 0; columnCount < 9; ++columnCount) {
            bool isWhite = board[rowCount][columnCount].white;
            if(isWhite) {
                current.push_back(rowCount * 9 + columnCount);  // i used the encoding row*9+column as we are not allowed to use the pair class.
            }
            if(!isWhite && !current.empty()) {
                compartments.push_back(current);
                current.clear();
            }
        }
        if(!current.empty()) {
            compartments.push_back(current);
            current.clear();
        }
    }
}

void findCompartmentsColumn() {
    vector<int> current;
    for(int columnCount = 0; columnCount < 9; ++columnCount) {
        for(int rowCount = 0; rowCount < 9; ++rowCount) {
            bool isWhite = board[rowCount][columnCount].white;
            if(isWhite) {
                current.push_back(rowCount * 9 + columnCount);  // i used the encoding row*9+column as we are not allowed to use the pair class.
            }
            if(!isWhite && !current.empty()) {
                compartments.push_back(current);
                current.clear();
            }
        }
        if(!current.empty()) {
            compartments.push_back(current);
            current.clear();
        }
    }
}

bool findMostConstrainedCell(int & bestRow, int & bestColumn) {
    int fewest = 10;
    bool found = false;
    for(int row = 0; row < 9; ++row) {
        for(int column = 0; column < 9; ++column) {
            SetSolverSquareSet & square  = board[row][column];
            int size = (int)square.set.size();
            if(square.white && size > 1 && size < fewest) {
                bestRow = row;
                bestColumn = column;
                fewest = size;
                found = true;
            }
        }
    }
    return found;
}

bool propagate() {
    bool isChanged = true;
    while(isChanged) {
        isChanged = false;
        if(applyRule1(isChanged) == false) {return false;}
        if(applyRule2(isChanged) == false) {return false;}
    }
    return true;
}

void writeSolvedValues() {
    for(int row = 0; row < 9; ++row) {
        for(int column = 0; column < 9; ++column) {
            SetSolverSquareSet & square = board[row][column];
            if(square.white && square.value == 99 && square.set.size() == 1) {
                square.value = square.set[0];
            }
        }
    }
}

bool search() {
    if(propagate() == false) {return false;}
    int row = 0;
    int column = 0;
    if(findMostConstrainedCell(row, column) == false) {return true;}
    vector<int> options = board[row][column].set;
    for(int option : options) {
        auto boardCopy = board;
        board[row][column].set = {option};
        if(search() == true) {return true;}
        board = boardCopy;
    }
    return false;
}
    
public:

void PopulateBoard(vector<string> skeletonBoard)
{
    board.assign(9, vector<SetSolverSquareSet>(9));
    int rowCounter = 0;
    int strCounter = 0;
    int indexCounter = 0;
    while(rowCounter < 9) {
        strCounter = 0;
        indexCounter = 0;
        const string & str = skeletonBoard[rowCounter];
        while(strCounter < (int)str.size()) {
            if(str[strCounter] == '-') {
                SetSolverSquareSet & square = board[rowCounter][indexCounter];
                square.white = false;
                square.value = -(str[strCounter + 1] - '0');
                square.set.clear();
                strCounter += 2;
                ++indexCounter;
            }
            else if(str[strCounter] == '0') {
                SetSolverSquareSet & square = board[rowCounter][indexCounter];
                square.white = false;
                square.value = 0;
                square.set.clear();
                ++strCounter;
                ++indexCounter;
            }
            else if(str[strCounter] == '*') {
                // the default constructor set the correct values already
                ++strCounter;
                ++indexCounter;
            }
            else {
                SetSolverSquareSet & square = board[rowCounter][indexCounter];
                square.white = true;
                square.value = (str[strCounter] - '0');
                square.set = {str[strCounter] - '0'};
                ++strCounter;
                ++indexCounter;
            }
        }
        ++rowCounter;
    }
    compartments.clear();
    findCompartmentsRow();
    findCompartmentsColumn();
}

int ReturnValue(size_t row, size_t col)
{
    return board[row][col].value;
}
    
void Solve()
{
    search();
    writeSolvedValues();
}
    
    
};
#endif /* SetSolver_h */
