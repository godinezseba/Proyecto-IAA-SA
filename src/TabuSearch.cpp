#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Entities/TabuList.cpp"
#include "./Movements/SwapHomes.cpp"
#include "./Movements/SwapMatchRounds.cpp"
#include "./Movements/SwapMatches.cpp"
#include "./Objective.cpp"

using namespace std;

TSTournament Movement(vector<vector<int>> distances, TSTournament scheduling, TabuTail<TabuSwapWithList> &tabuListSwapMatch, TabuTail<TabuSwapWithList> &tabuListSwapMatchRound, int DEBUG=0){
    unsigned int totalTeams = scheduling.getSchedule()[0].size();
    unsigned int totalRounds = scheduling.getSchedule().size();
    unsigned long int auxResult;
    TabuSwapWithList bestValues;
    bestValues.auxA = -1; // to check if a movement enter
    TabuSwapWithList tempValues;
    vector<vector<int>> bestActual = scheduling.getSchedule();
    SwapSolutions tempScheduling;

    // swap matches movement
    for(unsigned int auxTeamA = 1; auxTeamA <= totalTeams; auxTeamA++)
        for (unsigned int auxTeamB = auxTeamA+1; auxTeamB <= totalTeams; auxTeamB++)
            for(unsigned int auxRound = 1; auxRound <= totalRounds; auxRound++){
                tempValues.auxA = auxTeamA;
                tempValues.auxB = auxTeamB;
                tempValues.list = vector<int>();
                tempValues.list.push_back(auxRound);
                if(!tabuListSwapMatch.InTabuTail(AuxCondition, tempValues)){
                    tempScheduling = SwapMatches(bestActual, auxTeamA, auxTeamB, auxRound);
                    if(tempScheduling.movements.size() != 0){
                        auxResult = ObjectiveFunction(distances, tempScheduling.solution);
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

    unsigned long int bestResultSwapMatches = scheduling.getDistance();
    // swap matches round movement
    for(unsigned int auxRoundA = 1; auxRoundA <= totalRounds; auxRoundA++)
        for (unsigned int auxRoundB = auxRoundA+1; auxRoundB <= totalRounds; auxRoundB++)
            for(unsigned int auxTeam = 1; auxTeam <= totalTeams; auxTeam++){
                tempValues.auxA = auxRoundA;
                tempValues.auxB = auxRoundB;
                tempValues.list = vector<int>();
                tempValues.list.push_back(auxTeam);
                if(!tabuListSwapMatchRound.InTabuTail(AuxCondition, tempValues)){
                    tempScheduling = SwapMatchRounds(bestActual, auxRoundA, auxRoundB, auxTeam);
                    if(tempScheduling.movements.size() != 0){
                        auxResult = ObjectiveFunction(distances, tempScheduling.solution);
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
    
    // check what is the best
    if(bestResultSwapMatches > scheduling.getDistance()){ // swap matchround is better
        tabuListSwapMatchRound.addElement(bestValues);
        if(DEBUG) 
            cout << "[DEBUG] roundA: " << bestValues.auxA << ", roundB: " 
            << bestValues.auxB << ", team: " << bestValues.list[0] << ", result: " << scheduling.getDistance() << endl;
    } else if(bestValues.auxA != -1) { // the fisrt movement best result is better
        tabuListSwapMatch.addElement(bestValues);
        cout << "hola" << endl;
        if(DEBUG) 
            cout << "[DEBUG] roundA: " << bestValues.auxA << ", roundB: " 
            << bestValues.auxB << ", team: " << bestValues.list[0] << ", result: " << scheduling.getDistance() << endl;
    }

    return scheduling;
}

vector<vector<int>> BestResult(vector<vector<int>> distances, TSTournament scheduling, int iterations, int lenList, int DEBUG=0){
    TabuTail<TabuSwapWithList> tabuListSwapMatch = TabuTail<TabuSwapWithList>(lenList);
    TabuTail<TabuSwapWithList> tabuListSwapMatchRound = TabuTail<TabuSwapWithList>(lenList);

    for(int i = 0; i < iterations; i++){
        scheduling = Movement(distances, scheduling, tabuListSwapMatch, tabuListSwapMatchRound, DEBUG);
        if(DEBUG){
            scheduling.print();
            tabuListSwapMatch.print(printAuxiliar);
            tabuListSwapMatchRound.print(printAuxiliar);
        }
    }

    if(DEBUG) ObjectiveFunction(distances, scheduling.getSchedule(), DEBUG);

    return scheduling.getSchedule();
}

vector<vector<int>> TabuSearch(vector<vector<int>> distances, int iterations, int lenList, int DEBUG=0){
    int total = distances.size();

    TSTournament tournament = TSTournament(total, 1);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule()));
    tournament.print();

    // code to iterate with the movements separately
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapHomes, iterations, lenList, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapMatches, iterations, lenList, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapMatchRounds, iterations, lenList, DEBUG);

    // code to iterate swap matches with swap matchesround
    vector<vector<int>> tempSchedule = BestResult(distances, tournament, iterations, lenList, DEBUG);
    tournament.setSchedule(tempSchedule);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule(), 1));
    // tournament.print();

    return tournament.getSchedule();
}