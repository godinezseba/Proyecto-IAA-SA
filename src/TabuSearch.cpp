#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Auxiliar.cpp"

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
void SwapRival(vector<vector<int>> &scheduling, int roundA, int roundB, int team){
    // save the value
    int tempTeam = scheduling[roundA][team];
    // change positions
    scheduling[roundA][team] = scheduling[roundB][team];
    scheduling[roundB][team] = tempTeam;
}

/**
 * auxiliar movement to change the rivals in a pair of teams
 * input: movTeams: list with the teams that swap the matches
*/
unsigned int SwapRivals(vector<vector<int>> &scheduling, vector<int> &movTeams,int roundA, int roundB, unsigned int teamA){
    // change value for the first team
    SwapRival(scheduling, roundA, roundB, teamA);
    movTeams.push_back(teamA+1); // add team to the list

    if(DEBUG) cout << "movement:\nroundA: " << roundA+1 << " roundB: " << roundB+1 << " team: " << teamA+1 << endl;
    if(DEBUG) showSolution(scheduling);

    // find the second team to change the values 
    unsigned int teamB = abs(scheduling[roundB][teamA])-1;
    SwapRival(scheduling, roundA, roundB, teamB);
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
    // get enemys
    int enemyA = scheduling[roundA][team];
    int enemyB = scheduling[roundB][team];
    // find if the enemies are the same
    if(abs(enemyA) == abs(enemyB)){
        return solution;
    }
    // temporal value to change the rivals
    int b = team;
    do
        b = SwapRivals(scheduling, solution.movements, roundA, roundB, b);    
    while (b != team); // when the temporal team is the original the algorithm stop

    solution.solution = scheduling;

    if(DEBUG){
        for(int team : solution.movements) cout << team << " ";
        cout << endl;
    }

    return solution;
}

vector<vector<int>> TabuSearch(vector<vector<int>> distances){
    total = distances.size();

    TSTournament tournament = TSTournament(total, DEBUG);
    showSolution(tournament.getSchedule());
    // showSolution(SwapHomes(tournament.getSchedule(), 1, 2));
    showSolution(SwapMatchRounds(tournament.getSchedule(), 1, 5, 2).solution);
    return tournament.getSchedule();
}