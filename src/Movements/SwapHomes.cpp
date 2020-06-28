#include <vector>
#include <math.h>
#include "../Entities/Tournament.cpp"
#include "../Objective.cpp"

using namespace std;

struct TabuSwapHomes{
    int teamA;
    int teamB;
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

bool InTabuList(vector<TabuSwapHomes> tabuList, int teamA, int teamB){
  for(TabuSwapHomes pair : tabuList){
    bool cond1 = (pair.teamA == teamA && pair.teamB == teamB);
    bool cond2 = (pair.teamA == teamB && pair.teamB == teamA);
    if(cond1 || cond2)
      return true;
  }
  return false;
}

TSTournament BestSwapHomes(vector<vector<int>> distances, TSTournament scheduling, vector<TabuSwapHomes> &tabuList){
  unsigned int totalTeams = scheduling.getSchedule()[0].size();
  unsigned long int bestResult = scheduling.getDistance();
  unsigned long int auxResult;
  TabuSwapHomes tempValues;
  vector<vector<int>> tempScheduling;

  for (unsigned int auxTeamA = 0; auxTeamA < totalTeams; auxTeamA++)
    for (unsigned int auxTeamB = auxTeamA; auxTeamB < totalTeams; auxTeamB++)
      if(!InTabuList(tabuList, auxTeamA, auxTeamB)){
        // get scheduling from movement
        tempScheduling = SwapHomes(scheduling.getSchedule(), auxTeamA, auxTeamB);
        auxResult = ObjectiveFunction(distances, tempScheduling);
        // compare
        if(auxResult < scheduling.getDistance()){
          scheduling.setDistance(auxResult);
          scheduling.setSchedule(tempScheduling);
          tempValues.teamA = auxTeamA;
          tempValues.teamB = auxTeamB;
        }
      }

  if (bestResult != scheduling.getDistance()){ // add the result to the list
    if (tabuList.size() == 2){
      tabuList.erase(tabuList.begin());
    }
    tabuList.push_back(tempValues);
  }
  
  return scheduling;
}

vector<vector<int>> TabuSearchSwapHomes(vector<vector<int>> distances, TSTournament scheduling, int DEBUG=0){
  vector<TabuSwapHomes> tabuList;

  for(int i = 0; i < 100; i++){
    scheduling = BestSwapHomes(distances, scheduling, tabuList);
    if(DEBUG) scheduling.print();
  }

  return scheduling.getSchedule();
}