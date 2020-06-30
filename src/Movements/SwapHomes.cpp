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

void printHomes(TabuSwapHomes element){
  cout << "Equipo A: " << element.teamA << ", Equipo B: " << element.teamB << endl;
}

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

TSTournament BestSwapHomes(vector<vector<int>> distances, TSTournament scheduling, TabuTail<TabuSwapHomes> &tabuList, int DEBUG=0){
  unsigned int totalTeams = scheduling.getSchedule()[0].size();
  unsigned long int bestResult = scheduling.getDistance();
  unsigned long int auxResult;
  TabuSwapHomes tempValues;
  TabuSwapHomes bestValues;
  vector<vector<int>> bestActual = scheduling.getSchedule();
  vector<vector<int>> tempScheduling;

  for (unsigned int auxTeamA = 1; auxTeamA <= totalTeams; auxTeamA++)
    for (unsigned int auxTeamB = auxTeamA+1; auxTeamB <= totalTeams; auxTeamB++){

      tempValues.teamA = auxTeamA;
      tempValues.teamB = auxTeamB;
    
      if(!tabuList.InTabuTail(SwapHomesCondition, tempValues)){
        // get scheduling from movement
        tempScheduling = SwapHomes(bestActual, auxTeamA, auxTeamB);
        auxResult = ObjectiveFunction(distances, tempScheduling);
        // if(DEBUG) cout << "[DEBUG] equipoA: " << auxTeamA << ", equipoB: " << auxTeamB << ", result: " << auxResult << endl;
        // compare
        if(auxResult < scheduling.getDistance()){
          scheduling.setDistance(auxResult);
          scheduling.setSchedule(tempScheduling);
          bestValues = tempValues;
        }
      }
    }

  // add the result to the list
  if (bestResult != scheduling.getDistance()) tabuList.addElement(bestValues);
  
  // if(DEBUG) tabuList.print(printHomes);
  return scheduling;
}
