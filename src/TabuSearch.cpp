#include <vector>
#include <math.h>
#include "./Entities/Tournament.cpp"
#include "./Auxiliar.cpp"
#include "./Objective.cpp"

using namespace std;

int DEBUG;
int total;

/**
 * struct to save the solution and the list
 * for SwapMatchRounds
*/
struct SwapSolutions{
    vector<vector<int>> solution;
    vector<int> movements;
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
unsigned int SwapTeamsRival(vector<vector<int>> &scheduling, vector<int> &movTeams,int roundA, int roundB, unsigned int teamA){
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
SwapSolutions SwapMatchRounds(vector<vector<int>> scheduling, int roundA, int roundB, int team){
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
        b = SwapTeamsRival(scheduling, solution.movements, roundA, roundB, b);    
    while (b != team); // when the temporal team is the original the algorithm stop
    // add the new solution to the struct
    solution.solution = scheduling;

    if(DEBUG){
        for(int team : solution.movements) cout << team << " ";
        cout << endl;
    }

    return solution;
}

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
SwapSolutions SwapMatches(vector<vector<int>> scheduling, int teamA, int teamB, unsigned int round){
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

vector<vector<int>> TabuSearch(vector<vector<int>> distances){
    SwapSolutions temp;
    total = distances.size();

    TSTournament tournament = TSTournament(total, DEBUG);
    showSolution(tournament.getSchedule());
    // showSolution(SwapHomes(tournament.getSchedule(), 1, 2));
    // showSolution(SwapMatchRounds(tournament.getSchedule(), 1, 5, 2).solution);
    temp = SwapMatches(tournament.getSchedule(), 1, 3, 3);
    showSolution(temp.solution);
    Distance(temp.solution, distances, DEBUG);

    return tournament.getSchedule();
}