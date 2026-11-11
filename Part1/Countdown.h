#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <string>
#include <sstream>

// this is provided code for the last part of the README

std::string intToString(const int x) {
    std::ostringstream str;
    str << x;
    return str.str();
}

class CountdownSolution {
  
private:
    std::string solution;
    int value;
    
public:
    
    CountdownSolution() : solution(), value(0) {}
    
    CountdownSolution(const std::string & solutionIn, const int valueIn)
        : solution(solutionIn), value(valueIn) {
    }
    
    const std::string & getSolution() const {
        return solution;
    }
    
    int getValue() const {
        return value;
    }
    
};

// Do not edit above this line

// TODO: write code here:
using std::string;

using std::istringstream;

#include <vector>
using std::vector;



double evaluateCountdown(const string & input) {
    istringstream stream(input);
    vector<double> stack;
    string token;
    while(stream >> token) {
        if(token == "-") {
            double num1 = stack.back();
            stack.pop_back();
            double num2 = stack.back();
            stack.pop_back();
            stack.push_back(num2-num1);
        }
        else if(token == "+") {
            double num1 = stack.back();
            stack.pop_back();
            double num2 = stack.back();
            stack.pop_back();
            stack.push_back(num2+num1);
        }
        else if(token == "/") {
            double num1 = stack.back();
            stack.pop_back();
            double num2 = stack.back();
            stack.pop_back();
            stack.push_back(num2/num1);
        }
        else if(token == "*") {
            double num1 = stack.back();
            stack.pop_back();
            double num2 = stack.back();
            stack.pop_back();
            stack.push_back(num2*num1);
        }
        else {
            double number = std::stod(token);
            stack.push_back(number);
        }
        
    }
    return stack.back();
}

// solveCountdownProblem FUNCTION

int biggerNumber(int a, int b) {
    if(a > b) {return a;}
    return b;
}

void search(const vector<CountdownSolution> & pool, const int & target, CountdownSolution & bestSoFar) {
    if(bestSoFar.getValue() == target) {return;}

    for(const CountdownSolution & item : pool) {
        int absoluteDifferenceItem = biggerNumber((item.getValue() - target), (target - item.getValue()));
        int absoluteDifferenceBest = biggerNumber((bestSoFar.getValue() - target), (target - bestSoFar.getValue()));
        if(absoluteDifferenceItem < absoluteDifferenceBest) {bestSoFar = item;}
    }

    if(pool.size() < 2) {return;}

    for(int i = 0; i < pool.size(); ++i) {
        for(int j = 0; j < pool.size(); ++j) {
            const CountdownSolution & item1 = pool[i];
            const CountdownSolution & item2 = pool[j];
            if(i == j) {continue;}

            CountdownSolution newItemAddition(item1.getSolution() + " " + item2.getSolution() + " " + "+", item1.getValue() + item2.getValue());
            vector<CountdownSolution> newPool{newItemAddition};
            for(int k = 0; k < pool.size(); ++k) {
                if(k != i && k != j) {newPool.push_back(pool[k]);}
            }
            search(newPool, target, bestSoFar);
            newPool.clear();

            if(item1.getValue() > item2.getValue()) {
                CountdownSolution newItemSubtraction(item1.getSolution() + " " + item2.getSolution() + " " + "-", item1.getValue() - item2.getValue());
                newPool.push_back(newItemSubtraction);
                for(int k = 0; k < pool.size(); ++k) {
                    if(k != i && k != j) {newPool.push_back(pool[k]);}
                }
                search(newPool, target, bestSoFar);
            }
            newPool.clear();

            long long boundaryCheck = (long long) item1.getValue() * item2.getValue();
            if(boundaryCheck < 2147000000) {
                CountdownSolution newItemMultiplication(item1.getSolution() + " " + item2.getSolution() + " " + "*", item1.getValue() * item2.getValue());
                newPool.push_back(newItemMultiplication);
                for(int k = 0; k < pool.size(); ++k) {
                    if(k != i && k != j) {newPool.push_back(pool[k]);}
                }
                search(newPool, target, bestSoFar);
                newPool.clear();}
            
            if(item2.getValue() != 0 && (item1.getValue() % item2.getValue()) == 0) {
                CountdownSolution newItemDivision(item1.getSolution() + " " + item2.getSolution() + " " + "/", item1.getValue() / item2.getValue());
                newPool.push_back(newItemDivision);
                for(int k = 0; k < pool.size(); ++k) {
                    if(k != i && k != j) {newPool.push_back(pool[k]);}
                }
                search(newPool, target, bestSoFar);
            }
        }
    }
}

CountdownSolution solveCountdownProblem(const vector<int> & numbers, const int target) {
    vector<CountdownSolution> pool;
    for(int number : numbers) {
        pool.push_back(CountdownSolution(intToString(number), number));
    }

    CountdownSolution bestSoFar = pool[0];

    search(pool, target, bestSoFar);

    return bestSoFar;
}


// Do not edit below this line


#endif
