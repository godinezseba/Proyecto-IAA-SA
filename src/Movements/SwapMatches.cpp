#include <vector>
#include <math.h>
#include "../Auxiliar.cpp"

struct TabuSwapMatch{
    int teamA;
    int teamB;
    vector<int> rounds;
};

/**
 * Auxiliar movement to change the rivals for two teams in a round
*/
void SwapRoundRival(vector<vector<int>> &scheduling, int teamA, int teamB, int round){
    // save the value
    int tempTeam = scheduling[round][teamA];
    int rivalA = abs(tempTeam)-1;
    int rivalB = abs(scheduling[round][teamB])-1;
    int tempRivalTeam = scheduling[round][rivalA];
    // change positions, conserve sign
    scheduling[round][teamA] = scheduling[round][teamB];
    scheduling[round][teamB] = tempTeam;
    // change rival positions, change sign
    scheduling[round][rivalA] = copysign(scheduling[round][rivalB], -1*scheduling[round][teamB]);
    scheduling[round][rivalB] = copysign(tempRivalTeam, -1*scheduling[round][teamA]);
}

/**
 * Movement to change the rivals for two teams in a round
*/
SwapSolutions SwapMatches(vector<vector<int>> scheduling, int teamA, int teamB, unsigned int round, int DEBUG=0){
    SwapSolutions solution;
    // get position for the teams
    int posA = teamA - 1;
    int posB = teamB - 1;
    round--;

    // find if they play with each other
    if(abs(scheduling[round][posA]) == teamB)
        return solution;
    // else make the movement
    bool stopFlag;
    unsigned int tempRound = round;
    solution.movements.push_back(tempRound+1);
    do{
        stopFlag = true;
        // make the swap ta - a <-> b - tb to: ta - b and a - tb
        SwapRoundRival(scheduling, posA, posB, tempRound);
        if(DEBUG){
            if(DEBUG) cout << "movement:\nteamA: " << teamA << " teamB: " << teamB << " round: " << tempRound+1 << endl;
            showSolution(scheduling);
        }

        int b = scheduling[tempRound][posA];
        // find the other round where is ta - b
        unsigned int auxRound;
        for(auxRound = 0; auxRound < scheduling.size(); auxRound++)
            if(auxRound != tempRound && auxRound != round && scheduling[auxRound][posA] == b){
                stopFlag = false;
                tempRound = auxRound;
                solution.movements.push_back(tempRound+1);
                break; // stop finding
            }
    } while (!stopFlag);
    solution.solution = scheduling;

    if(DEBUG){
        for(int team : solution.movements) cout << team << " ";
        cout << endl;
    }
    
    return solution;
}
