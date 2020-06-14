#include <vector>
#include "./Entities/Tournament.cpp"

using namespace std;

int total;

vector<vector<int>> TabuSearch(vector<vector<int>> distances, int DEBUG){
  total = distances.size();

  TSTournament tournament = TSTournament(total, DEBUG);
  return tournament.getSchedule();
}