#include <vector>
#include "./Entities/Tournament.cpp"
#include "./Entities/TabuList.cpp"
#include "./Movements/SwapHomes.cpp"
#include "./Movements/SwapMatchRounds.cpp"
#include "./Movements/SwapMatches.cpp"
#include "./Objective.cpp"

using namespace std;

vector<vector<int>> TabuSearchThreeSwaps(vector<vector<int>> distances, 
                                        TSTournament scheduling,
                                        int iterations, 
                                        int lenList,
                                        int weight, 
                                        int DEBUG=0){
    TabuTail<TabuSwapHomes> tabuListHomes = TabuTail<TabuSwapHomes>(lenList);
    TabuTail<TabuSwapWithList> tabuListMatches = TabuTail<TabuSwapWithList>(lenList);
    TabuTail<TabuSwapWithList> tabuListRounds = TabuTail<TabuSwapWithList>(lenList);
    TabuSwapHomes tabuHomes;
    TabuSwapWithList tabuMatches;
    TabuSwapWithList tabuRounds;
    TSTournament tempHomes = scheduling;
    TSTournament tempMatches = scheduling;
    TSTournament tempRounds = scheduling;
    TSTournament bestTemp = scheduling;

    for (int i = 0; i < iterations; i++){
        tabuHomes = BestSwapHomes(distances, tempHomes, tabuListHomes, weight);
        tabuMatches = BestSwapMatches(distances, tempMatches, tabuListMatches, weight);
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
                tabuListMatches.addElement(tabuMatches);
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

    // code to iterate swap matches with swap matchesround
    // vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapHomes, BestSwapMatches, iterations, lenList, DEBUG);
    vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapHomes, BestSwapMatchRounds, iterations, lenList, weight, DEBUG);
    // vector<vector<int>> tempSchedule = TabuSearchTwoSwaps(distances, tournament, BestSwapMatches, BestSwapMatchRounds, iterations, lenList, DEBUG);
    
    // swap of the three movements
    // vector<vector<int>> tempSchedule = TabuSearchThreeSwaps(distances, tournament, iterations, lenList, DEBUG);

    tournament.setSchedule(tempSchedule);
    tournament.setDistance(ObjectiveFunction(distances, tournament.getSchedule(), weight, 0));
    tournament.print();

    return tournament.getSchedule();
}