#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Entities/TabuList.cpp"
#include "./Movements/SwapHomes.cpp"
#include "./Movements/SwapMatchRounds.cpp"
#include "./Movements/SwapMatches.cpp"
#include "./Objective.cpp"

using namespace std;

int DEBUG;

vector<vector<int>> TabuSearch(vector<vector<int>> distances){
    SwapSolutions temp;
    int total = distances.size();

    TSTournament tournament = TSTournament(total, 0);
    tournament.setDistance(Distance(distances, tournament.getSchedule()));
    tournament.print();

    showSolution(TabuSearchSwapHomes(distances, tournament, DEBUG));

    return tournament.getSchedule();
}