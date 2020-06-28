#include <vector>
#include <math.h>
#include "../Entities/Tournament.cpp"
#include "../Entities/TabuList.cpp"
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

bool SwapHomesCondition(TabuSwapHomes inList, TabuSwapHomes auxiliar){
  bool cond1 = (inList.teamA == auxiliar.teamA && inList.teamB == auxiliar.teamB);
  bool cond2 = (inList.teamA == auxiliar.teamB && inList.teamB == auxiliar.teamA);
  return cond1 || cond2;
}

TSTournament BestSwapHomes(vector<vector<int>> distances, TSTournament scheduling, TabuTail<TabuSwapHomes> &tabuList){
  unsigned int totalTeams = scheduling.getSchedule()[0].size();
  unsigned long int bestResult = scheduling.getDistance();
  unsigned long int auxResult;
  TabuSwapHomes tempValues;
  TabuSwapHomes bestValues;
  vector<vector<int>> tempScheduling;

  for (unsigned int auxTeamA = 0; auxTeamA < totalTeams; auxTeamA++)
    for (unsigned int auxTeamB = auxTeamA; auxTeamB < totalTeams; auxTeamB++){
      tempValues.teamA = auxTeamA;
      tempValues.teamB = auxTeamB;
    
      if(!tabuList.InTabuTail(SwapHomesCondition, tempValues)){
        // get scheduling from movement
        tempScheduling = SwapHomes(scheduling.getSchedule(), auxTeamA, auxTeamB);
        auxResult = ObjectiveFunction(distances, tempScheduling);
        // compare
        if(auxResult < scheduling.getDistance()){
          scheduling.setDistance(auxResult);
          scheduling.setSchedule(tempScheduling);
          bestValues = tempValues;
        }
      }
    }

  // add the result to the list
  if (bestResult != scheduling.getDistance()) tabuList.addElement(tempValues);
  
  return scheduling;
}

vector<vector<int>> TabuSearchSwapHomes(vector<vector<int>> distances, TSTournament scheduling, int DEBUG=0){
  TabuTail<TabuSwapHomes> tabuList = TabuTail<TabuSwapHomes>(8);

  for(int i = 0; i < 100; i++){
    scheduling = BestSwapHomes(distances, scheduling, tabuList);
    if(DEBUG) scheduling.print();
  }

  ObjectiveFunction(distances, scheduling.getSchedule(), 1);

  return scheduling.getSchedule();
}