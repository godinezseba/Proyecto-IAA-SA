#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Entities/TabuList.cpp"
#include "./Movements/SwapHomes.cpp"
#include "./Movements/SwapMatchRounds.cpp"
#include "./Movements/SwapMatches.cpp"
#include "./Movements/SwapTeams.cpp"
#include "./Objective.cpp"

using namespace std;

vector<vector<int>> TabuSearchThreeSwaps(vector<vector<int>> distances, 
                                        TSTournament scheduling,
                                        int iterations, 
                                        int lenList,
                                        int weight, 
                                        int DEBUG=0){
    TabuTail<TabuSwapHomes> tabuListHomes = TabuTail<TabuSwapHomes>(lenList);
    TabuTail<TabuSwapTeams> tabuListTeams = TabuTail<TabuSwapTeams>(lenList);
    TabuTail<TabuSwapWithList> tabuListRounds = TabuTail<TabuSwapWithList>(lenList);
    TabuSwapHomes tabuHomes;
    TabuSwapTeams tabuTeams;
    TabuSwapWithList tabuRounds;
    TSTournament tempHomes = scheduling;
    TSTournament tempMatches = scheduling;
    TSTournament tempRounds = scheduling;
    TSTournament bestTemp = scheduling;

    for (int i = 0; i < iterations; i++){
        tabuHomes = BestSwapHomes(distances, tempHomes, tabuListHomes, weight);
        tabuTeams = BestSwapTeams(distances, tempMatches, tabuListTeams, weight);
        tabuRounds = BestSwapMatchRounds(distances, tempRounds, tabuListRounds, weight);
        // check if one of the three is better
        if(tempHomes.getDistance() < bestTemp.getDistance() || tempMatches.getDistance() < bestTemp.getDistance() || tempRounds.getDistance() < bestTemp.getDistance()){
            if(tempHomes.getDistance() < tempMatches.getDistance() || tempHomes.getDistance() < tempRounds.getDistance()){
                bestTemp = tempHomes;
                tempMatches = tempHomes;
                tempRounds = tempHomes;
                tabuListHomes.addElement(tabuHomes);
            } else if(tempMatches.getDistance() < tempHomes.getDistance() || tempMatches.getDistance() < tempRounds.getDistance()){
                bestTemp = tempMatches;
                tempHomes = tempMatches;
                tempRounds = tempMatches;
                tabuListTeams.addElement(tabuTeams);
            } else{
                bestTemp = tempRounds;
                tempHomes = tempRounds;
                tempMatches = tempRounds;
                tabuListRounds.addElement(tabuRounds);
            }
        }
        if (DEBUG) bestTemp.print();
    }
    
    if(DEBUG) ObjectiveFunction(distances, bestTemp.getSchedule(), 0);


    return bestTemp.getSchedule();
}


vector<vector<int>> TabuSearch(vector<vector<int>> distances, int iterations, int lenList, int weight,int DEBUG=0){
    int total = distances.size();

    TSTournament tournament = TSTournament(total, 0);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule(), weight));
    // tournament.print();

    // code to iterate with the movements separately
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapHomes, iterations, lenList, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapMatches, iterations, lenList, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapMatchRounds, iterations, lenList, DEBUG);
    // new in the homework-3
    // vector<vector<int>> tempSchedule = TabuSearchOneSwap(distances, tournament, BestSwapTeams, iterations, lenList, weight, DEBUG);

    // code to iterate swap matches with swap matchesround
    // vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapHomes, BestSwapMatches, iterations, lenList, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapHomes, BestSwapMatchRounds, iterations, lenList, weight, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapMatches, BestSwapMatchRounds, iterations, lenList, DEBUG);
    // new in the homework-3
    vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapHomes, BestSwapTeams, iterations, lenList, weight, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapMatchRounds, BestSwapTeams, iterations, lenList, weight, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapMatches, BestSwapTeams, iterations, lenList, weight, DEBUG);
    
    // swap of the three movements
    // vector<vector<int>> tempSchedule = TabuSearchThreeSwaps(distances, tournament, iterations, lenList, DEBUG);

    // tournament.setSchedule(tempSchedule);
    // tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule(), weight, 0));
    // tournament.print();

    return tournament.getSchedule();
}