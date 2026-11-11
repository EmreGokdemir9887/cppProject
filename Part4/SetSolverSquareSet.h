#ifndef SetSolverSquareSet_h
#define SetSolverSquareSet_h

// Do not add any additional libraries.
#include <vector>


class SetSolverSquareSet
{
public:
    std::vector<int>set = {1,2,3,4,5,6,7,8,9};
    bool white = true;
    int value = 99;

    SetSolverSquareSet() = default;

    bool contains(int searchingFor) {
        for(int num : set) {
            if(num == searchingFor) {
                return true;
            }
        }
        return false;
    }

    bool remove(int removeNumber) {
        for(int i = 0; i < (int)set.size(); ++i) {
            if(set[i] == removeNumber) {
                set.erase(set.begin() + i);
                return true;
            }
        }
        return false;
    }
};

#endif /* SetSolverSquareSet_h */
