#include <vector>
#include <math.h>
#include "../Entities/TabuList.cpp"
#include "../Entities/Tournament.cpp"
#include "../Entities/AuxiliarTabuEntities.cpp"

/**
 * Auxiliar movement to change the rivals for two teams in a round
*/
void SwapRoundRival(vector<vector<int>> &scheduling, int teamA, int teamB, int round){
    // save the value
    int tempTeam = scheduling[round][teamA];
    int rivalA = abs(tempTeam)-1;
    int rivalB = abs(scheduling[round][teamB])-1;
    int tempRivalTeam = scheduling[round][rivalA];
    // change positions, conserve sign
    scheduling[round][teamA] = scheduling[round][teamB];
    scheduling[round][teamB] = tempTeam;
    // change rival positions, change sign
    scheduling[round][rivalA] = copysign(scheduling[round][rivalB], -1*scheduling[round][teamB]);
    scheduling[round][rivalB] = copysign(tempRivalTeam, -1*scheduling[round][teamA]);
}

/**
 * Movement to change the rivals for two teams in a round
*/
SwapSolutions SwapMatches(vector<vector<int>> scheduling, int teamA, int teamB, unsigned int round, int DEBUG=0){
    SwapSolutions solution;
    // get position for the teams
    int posA = teamA - 1;
    int posB = teamB - 1;
    round--;

    // find if they play with each other
    if(abs(scheduling[round][posA]) == teamB)
        return solution;
    // else make the movement
    bool stopFlag;
    unsigned int tempRound = round;
    solution.movements.push_back(tempRound+1);
    do{
        stopFlag = true;
        // make the swap ta - a <-> b - tb to: ta - b and a - tb
        SwapRoundRival(scheduling, posA, posB, tempRound);
        if(DEBUG){
            if(DEBUG) cout << "movement:\nteamA: " << teamA << " teamB: " << teamB << " round: " << tempRound+1 << endl;
            showSolution(scheduling);
        }

        int b = scheduling[tempRound][posA];
        // find the other round where is ta - b
        unsigned int auxRound;
        for(auxRound = 0; auxRound < scheduling.size(); auxRound++)
            if(auxRound != tempRound && auxRound != round && scheduling[auxRound][posA] == b){
                stopFlag = false;
                tempRound = auxRound;
                solution.movements.push_back(tempRound+1);
                break; // stop finding
            }
    } while (!stopFlag);
    solution.solution = scheduling;

    if(DEBUG){
        for(int team : solution.movements) cout << team << " ";
        cout << endl;
    }
    
    return solution;
}

TSTournament BestSwapMatches(vector<vector<int>> distances, TSTournament scheduling, TabuTail<TabuSwapWithList> &tabuList, int DEBUG=0){
    unsigned int totalTeams = scheduling.getSchedule()[0].size();
    unsigned int totalRounds = scheduling.getSchedule().size();
    unsigned long int bestResult = scheduling.getDistance();
    unsigned long int auxResult;
    TabuSwapWithList bestValues;
    TabuSwapWithList tempValues;
    vector<vector<int>> bestActual = scheduling.getSchedule();
    SwapSolutions tempScheduling;

    for(unsigned int auxTeamA = 1; auxTeamA <= totalTeams; auxTeamA++)
        for (unsigned int auxTeamB = auxTeamA+1; auxTeamB <= totalTeams; auxTeamB++)
            for(unsigned int auxRound = 1; auxRound <= totalRounds; auxRound++){
                tempValues.auxA = auxTeamA;
                tempValues.auxB = auxTeamB;
                tempValues.list = vector<int>();
                tempValues.list.push_back(auxRound);
                if(!tabuList.InTabuTail(AuxCondition, tempValues)){
                    tempScheduling = SwapMatches(bestActual, auxTeamA, auxTeamB, auxRound);
                    if(tempScheduling.movements.size() != 0){
                        auxResult = ObjectiveFunction(distances, tempScheduling.solution);
                        if(DEBUG) cout << "[DEBUG] equipoA: " << auxTeamA << ", equipoB: " << auxTeamB << ", round: " << auxRound << ", result: " << auxResult << endl;
                        // compare
                        if(auxResult < scheduling.getDistance()){
                            scheduling.setDistance(auxResult);
                            scheduling.setSchedule(tempScheduling.solution);
                            tempValues.list = tempScheduling.movements;
                            bestValues = tempValues;
                        }
                    }
                }                
            }
    // add the result to the list
    if (bestResult != scheduling.getDistance()) tabuList.addElement(bestValues);
    
    if(DEBUG) tabuList.print(printAuxiliar);
    return scheduling;
}
