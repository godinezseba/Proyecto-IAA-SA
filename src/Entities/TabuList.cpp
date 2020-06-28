#if !defined(TabuList)
#define TabuList

#include <iostream>
#include <vector>

using namespace std;

/**
 * struct to save the solution and the list
 * for SwapMatchRounds and SwapMatches
*/
struct SwapSolutions{
    vector<vector<int>> solution;
    vector<int> movements;
};

#endif