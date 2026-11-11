# Algorithms and Data Structures in C++

Four C++ programming tasks from my coursework at King's College London, each written on my own in C++11. They cover a search problem, dynamic programming, a data structure built from scratch, and a puzzle solver.

| Part | Task | Main ideas |
|------|------|------------|
| 1 | Countdown numbers solver | Recursive search, reverse Polish notation, overflow safety |
| 2 | Cheapest way to build a string | Dynamic programming |
| 3 | Doubly linked list | Templates, iterators, manual memory management |
| 4 | Str8ts style puzzle solver | Constraint propagation, backtracking |

## Part 1: Countdown solver (`Part1/Countdown.h`)

In the Countdown game you get six numbers and a target, and you combine the numbers with `+`, `-`, `*` and `/` to get as close to the target as you can.

- `evaluateCountdown` reads an expression written in reverse Polish notation (for example `100 4 + 9 +`) and works out its value using a stack.
- `solveCountdownProblem` searches through every way of combining the numbers. It keeps the best answer found so far and stops early if it hits the target exactly.
- Each number can be used once, including when the same value appears twice. Subtraction must give a positive result and division must be exact.
- Products are calculated with 64 bit integers first, so very large values are discarded before they can overflow.

It passed all 8 of the module's automated tests. My slowest test cases took about 5 to 7 seconds on a MacBook Air, against a limit of 240 seconds.

## Part 2: String construction (`Part2/StringConstruction.h`)

You build a target string from an empty one. Each step either appends one character, or clones a piece of the string you have already built, and the two moves have different costs. The task is to find the cheapest total cost.

- Let `cost[j]` be the cheapest way to build the first `j` characters. Each value comes from the one before it, so one pass over the string is enough.
- A clone only counts if the piece being copied already exists inside the part of the string built so far.
- The "greedy" idea of always cloning the longest piece is wrong. The cheapest answer sometimes appends a character it could have cloned, so that a longer clone is possible next.
- The leftmost position that can start a valid clone only ever moves to the right as `j` grows, so the code never has to go back.

I checked it against a slow but simple version on thousands of random strings, and it handles strings of 30,000 characters in under 3 seconds.

## Part 3: Doubly linked list (`Part3/linkedlist.h`, `Part3/node.h`)

A linked list written from scratch as C++ templates, with no STL containers.

- `Node`, `NodeIterator` and `ConstNodeIterator`, so the list works with range based `for` loops, including on a `const` list.
- `LinkedList<T>` with `push_front`, `push_back`, `insert`, `erase`, `reverse`, `begin` and `end`, plus default, initializer list and copy constructors.
- A destructor that frees every node. Memory is managed by hand.
- `insert` and `erase` take and return iterators, and they handle the empty list, the front and the back as special cases.

```cpp
LinkedList<int> list = {1, 2, 3};
list.push_front(0);
list.reverse();
for (auto x : list) std::cout << x << ' ';   // 3 2 1 0
```

## Part 4: Str8ts style puzzle solver (`Part4/SetSolver.h`, `Part4/SetSolverSquareSet.h`)

A 9 by 9 board has black and white cells. Each run of white cells (a "compartment") must hold consecutive digits in any order. No digit may repeat in a row or column, and the numbers on black cells also cannot appear in their row or column.

- Every empty cell keeps a list of the digits it could still be.
- **Rule 1** removes any digit that is already known elsewhere in the same row or column.
- **Rule 2** looks at each compartment and removes digits that cannot be part of any valid run for that compartment's length and its known cells.
- The two rules repeat until nothing changes.
- If cells are still undecided, the solver picks the cell with the fewest options, tries each one, and undoes it if that leads to a contradiction. If no choice works, it stops and reports that there is no solution.

It solves all of the boards provided with the coursework.

## Using the code

The files are headers only. Each part was written to fit a test harness supplied by the module, which is not included here, so there is no `main` in this repository. To try a part, write a small `main.cpp` that includes the header and compile it with:

```
g++ -std=c++11 -o demo main.cpp
```

For example:

```cpp
#include <iostream>
#include "Part1/Countdown.h"
#include "Part2/StringConstruction.h"

int main() {
    CountdownSolution s = solveCountdownProblem({100, 4, 17, 9, 3, 2}, 37);
    std::cout << s.getSolution() << " = " << s.getValue() << "\n";   // 100 4 + 9 + 2 - 3 / = 37

    std::cout << stringConstruction("aaaa", 3, 2) << "\n";           // 7 (clone cost 3, append cost 2)
}
```

## What I learned

- Writing a recursive search while keeping a close eye on correctness, overflow and running time.
- Turning a problem into a table of smaller subproblems, and checking the result against a brute force version.
- Pointers and memory ownership, which the linked list forced me to think through properly.
- Using simple rules to shrink a search space before guessing.
