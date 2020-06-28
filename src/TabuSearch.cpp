#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Movements/SwapHomes.cpp"
#include "./Movements/SwapMatchRounds.cpp"
#include "./Movements/SwapMatches.cpp"
#include "./Auxiliar.cpp"
#include "./Objective.cpp"

using namespace std;

int DEBUG;
int total;

vector<vector<int>> TabuSearch(vector<vector<int>> distances){
    SwapSolutions temp;
    total = distances.size();

    TSTournament tournament = TSTournament(total, DEBUG);
    showSolution(tournament.getSchedule());
    showSolution(SwapHomes(tournament.getSchedule(), 1, 2));
    // showSolution(SwapMatchRounds(tournament.getSchedule(), 1, 5, 2).solution);
    temp = SwapMatches(tournament.getSchedule(), 1, 3, 3);
    // temp = SwapMatchRounds(tournament.getSchedule(), 1, 5, 2);
    showSolution(temp.solution);
    Distance(temp.solution, distances, 0);

    return tournament.getSchedule();
}