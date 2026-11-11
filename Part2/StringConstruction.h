#ifndef STRINGCONSTRUCTION_H
#define STRINGCONSTRUCTION_H

#include <string>
using std::string;

#include <vector>
using std::vector;

// TODO: your code goes here:


int returnMin(int a, int b) {
    if(a<=b){return a;}
    else{return b;}
}

int stringConstruction(const string & target, int cloneCost, int appendCost) {
    vector<int> cost(target.size() + 1);
    int i = 0;
    for(int j = 1; j <= (int)target.size(); ++j) {
        while(i<j) {
            string built = target.substr(0,i);
            string clone = target.substr(i,j-i);
            if(built.find(clone) != string::npos) {
                cost[j] = returnMin(cost[j-1] + appendCost, cost[i] + cloneCost);
                break;
            } else {
                ++i;
            }
            if(i == j) {
                cost[j] = cost[j - 1] + appendCost;
            }
        }
    }
    return cost[target.size()];
    
}

// target = xzxpzxzxpq
// j = 7
// i = 4
// 0 1  2  3  4  5  6  7 8 9 10
// 0 10 20 30 40 50 61 


// do not write or edit anything below this line

#endif
