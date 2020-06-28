#include <vector>
#include "../Auxiliar.cpp"

using namespace std;

struct TabuSwapMatchRounds{
    int roundA;
    int roundB;
    vector<int> teams;
};

/**
 * Auxiliar movement to change the rivals for a team in two rounds
*/
void SwapTeamRival(vector<vector<int>> &scheduling, int roundA, int roundB, int team){
    // save the value
    int tempTeam = scheduling[roundA][team];
    // change positions
    scheduling[roundA][team] = scheduling[roundB][team];
    scheduling[roundB][team] = tempTeam;
}

/**
 * auxiliar movement to change the rivals in a pair of teams
 * input: movTeams: list with the teams that swap the matches
 * output: the new point to continue the reparation
*/
unsigned int SwapTeamsRival(vector<vector<int>> &scheduling, vector<int> &movTeams,int roundA, int roundB, unsigned int teamA, int DEBUG){
    // change value for the first team
    SwapTeamRival(scheduling, roundA, roundB, teamA);
    movTeams.push_back(teamA+1); // add team to the list

    if(DEBUG) cout << "movement:\nroundA: " << roundA+1 << " roundB: " << roundB+1 << " team: " << teamA+1 << endl;
    if(DEBUG) showSolution(scheduling);

    // find the second team to change the values 
    unsigned int teamB = abs(scheduling[roundB][teamA])-1;
    SwapTeamRival(scheduling, roundA, roundB, teamB);
    movTeams.push_back(teamB+1);

    if(DEBUG) cout << "movement:\nroundA: " << roundA+1 << " roundB: " << roundB+1 << " team: " << teamB+1 << endl;
    if(DEBUG) showSolution(scheduling);

    return abs(scheduling[roundA][teamB]) - 1;
}

/**
 * movement that change the rivals for a team
 * input: the team, the two round to change the rival
 * output: a new scheduling that is double round-robin
*/
SwapSolutions SwapMatchRounds(vector<vector<int>> scheduling, int roundA, int roundB, int team, int DEBUG=0){
    SwapSolutions solution;
    // change position to work in the matrix
    roundA--;
    roundB--;
    team--;
    // get enemies
    int enemyA = scheduling[roundA][team];
    int enemyB = scheduling[roundB][team];
    // find if the enemies are the same
    if(abs(enemyA) == abs(enemyB))
        return solution;
    // else make the swap
    // temporal value to change the rivals
    int b = team;
    do // this while is to make the repair chain
        b = SwapTeamsRival(scheduling, solution.movements, roundA, roundB, b, DEBUG);    
    while (b != team); // when the temporal team is the original the algorithm stop
    // add the new solution to the struct
    solution.solution = scheduling;

    if(DEBUG){
        for(int team : solution.movements) cout << team << " ";
        cout << endl;
    }

    return solution;
}
