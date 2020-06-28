#include <vector>
#include <math.h>

using namespace std;

struct TabuSwapHomes{
    int teamA;
    int teamB;
};

/**
 * movement that change the local/visit for two teams
 * input: the two teams to swap
 * output: the new scheduling
*/
vector<vector<int>> SwapHomes(vector<vector<int>> scheduling, int teamA, int teamB){
    int postA = teamA - 1;
    int postB = teamB - 1;
    // find the two rounds and change the sign
    for(vector<int> &round : scheduling)
        if (abs(round[postA]) == teamB){
            round[postA] = -1*round[postA];
            round[postB] = -1*round[postB];
        }

    return scheduling;
}
