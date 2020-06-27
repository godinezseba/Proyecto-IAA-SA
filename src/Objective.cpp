#include <iostream>
#include <vector>

using namespace std;

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
        for (vector<int> round : solution) {
            int oponent = round[actualTeam];
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
        }
        if (actualCity != actualTeam) localDistance += distances[actualCity][actualTeam]; // if the team dont finish in home

        distance += localDistance;

        if(DEBUG) cout << "[DEBUG] distancia hasta punto " << actualTeam << " " << distance << endl;
    }

    if(DEBUG) cout << "[DEBUG] cantidad de faltas: " << penalty << endl;
    if(DEBUG) cout << "Distancia total: " << distance << endl;

    return distance;
}
