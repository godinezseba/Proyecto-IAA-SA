#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Entities/TabuList.cpp"
#include "./Movements/SwapHomes.cpp"
#include "./Movements/SwapMatchRounds.cpp"
#include "./Movements/SwapMatches.cpp"
#include "./Objective.cpp"

using namespace std;

int DEBUG;

vector<vector<int>> TabuSearch(vector<vector<int>> distances, int iterations){
    int total = distances.size();

    TSTournament tournament = TSTournament(total, 1);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule()));
    tournament.print();

    vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapMatches, iterations, DEBUG);
    tournament.setSchedule(tempSchedule);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule(), 1));
    tournament.print();

    return tournament.getSchedule();
}