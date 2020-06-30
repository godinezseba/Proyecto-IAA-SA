#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Entities/TabuList.cpp"
#include "./Movements/SwapHomes.cpp"
#include "./Movements/SwapMatchRounds.cpp"
#include "./Movements/SwapMatches.cpp"
#include "./Objective.cpp"

using namespace std;

vector<vector<int>> TabuSearch(vector<vector<int>> distances, int iterations, int lenList, int DEBUG=0){
    int total = distances.size();

    TSTournament tournament = TSTournament(total, 0);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule()));
    tournament.print();

    // code to iterate with the movements separately
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapHomes, iterations, lenList, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapMatches, iterations, lenList, DEBUG);
    vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapMatchRounds, iterations, lenList, DEBUG);
    tournament.setSchedule(tempSchedule);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule(), 0));
    // tournament.print();

    return tournament.getSchedule();
}