#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Auxiliar.cpp"

using namespace std;

int total;

vector<vector<int>> SwapHomes(vector<vector<int>> scheduling, int teamA, int teamB){
    int postA = teamA - 1;
    int postB = teamB - 1;
    // find the two rounds and change the sign
    for(vector<int> &round : scheduling)
        if (abs(round[teamA-1]) == teamB){
            round[postA] = -1*round[postA];
            round[postB] = -1*round[postB];
        }

    return scheduling;
}

vector<vector<int>> TabuSearch(vector<vector<int>> distances, int DEBUG){
  total = distances.size();

  TSTournament tournament = TSTournament(total, DEBUG);
  showSolution(tournament.getSchedule());
  showSolution(SwapHomes(tournament.getSchedule(), 1, 2));
  return tournament.getSchedule();
}