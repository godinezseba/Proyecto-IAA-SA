#ifndef Objective
#define Objective

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

float SmothFunction(int penalty){
    return  1 + (sqrt(penalty)*log(penalty))/2;
}

float DistancePenalty(int distance, int penalty){
    float f = SmothFunction(penalty);
    float weigth = 100*penalty;

    float powDistance = pow(distance, 2);
    float powPenalty = pow(weigth*f, 2);

    return sqrt(powDistance + powPenalty);
}

unsigned int Distance(vector<vector<int>> solution, vector<vector<int>> distances, int DEBUG) {
    unsigned int distance = 0;
    int localDistance = 0;
    long unsigned int actualCity; // the city where our team is
    int trip; // count how many times the team play as visit or local
    unsigned int penalty = 0;

    for (long unsigned int actualTeam = 0; actualTeam < solution[0].size(); actualTeam++) {
        trip = 0; // sum if a team has a local/visit trip > 3
        localDistance = 0;
        actualCity = actualTeam; // start point of the traveling
        for (long unsigned int round = 0; round < solution.size(); round++) {
            int oponent = solution[round][actualTeam];
            int oponentPost = abs(oponent) - 1;
            // if (actualCity == actualTeam && oponent < 0) don't sum
            if(actualCity != actualTeam && oponent < 0) { // distance from point to house
                localDistance += distances[actualCity][actualTeam];
                actualCity = actualTeam;
            } else if(oponent > 0) { // distance from house/foreign to other point
                localDistance += distances[actualCity][oponentPost];
                actualCity = oponentPost;
            }
            // penalty: sum to the trip
            if ((trip > 0 && oponent < 0) || (trip < 0 && oponent > 0)) trip = 0; // reset if necesary
            if (oponent > 0) trip++;
            else trip--;
            if (trip > 3 || trip < -3) penalty++;
            // penalty: a team cant play vs the same team in a row
            if(round < (solution.size()-1))
                if(abs(oponent) == abs(solution[round+1][actualTeam]))
                    penalty++;
        }
        if (actualCity != actualTeam) localDistance += distances[actualCity][actualTeam]; // if the team dont finish in home

        distance += localDistance;

        if(DEBUG) cout << "[DEBUG] distancia hasta punto " << actualTeam << " " << distance << endl;
    }

    if(DEBUG) cout << "[DEBUG] cantidad de faltas: " << penalty << endl;
    if(DEBUG) cout << "[DEBUG] Distancia total: " << distance << endl;

    if(penalty != 0){
        distance = DistancePenalty(distance, penalty);
        if(DEBUG) cout << "[DEBUG] Distancia con penalty: " << distance << endl;
    }

    return distance;
}

#endif