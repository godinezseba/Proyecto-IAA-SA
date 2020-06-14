#include <iostream>
#include <vector>

using namespace std;

int calculateDistance(vector<vector<int>> solution, vector<vector<int>> distances, int DEBUG) {
    int distance = 0;
    long unsigned int actual;
    long unsigned int i;

    for (i = 0; i < solution[0].size(); i++) {
        actual = i; // start point of the traveling
        for (long unsigned int j = 0; j < solution.size(); j++) {
            int oponentPost = abs(solution[j][i]) - 1;
            int oponent = solution[j][i];
            // if (actual == i && oponent < 0) don't sum
            if(actual != i && oponent < 0) { // distance from point to house
                distance += distances[actual][i];
                actual = i;
            } else if(oponent > 0) { // distance from house/foreign to other point
                distance += distances[actual][oponentPost];
                actual = oponentPost;
            }
        }
        if (actual != i) distance += distances[actual][i];
        if(DEBUG) cout << "[DEBUG] distancia hasta punto " << i << " " << distance << endl;
    }

    if(DEBUG) cout << "[DEBUG] Distancia total: " << distance << endl;
    
    return distance;
}

void calculateDistancePenalty(vector<vector<int>> solution, vector<vector<int>> distances, int DEBUG) {
    int distance = 0;
    int localDistance = 0;
    long unsigned int actual;
    long unsigned int i;
    int trip;
    bool flagTrip;


    for (i = 0; i < solution[0].size(); i++) {
        trip = 0; // sum if a team has a local/visit trip > 3
        localDistance = 0;
        actual = i; // start point of the traveling
        flagTrip = false;
        for (long unsigned int j = 0; j < solution.size(); j++) {
            int oponentPost = abs(solution[j][i]) - 1;
            int oponent = solution[j][i];
            //if (actual == i && oponent < 0) don't sum
            if(actual != i && oponent < 0) { // distance from point to house
                localDistance += distances[actual][i];
                actual = i;
            } else if(oponent > 0) { // distance from house/foreign to other point
                localDistance += distances[actual][oponentPost];
                actual = oponentPost;
            }
            // sum to the trip
            if ((trip > 0 && oponent < 0) || (trip < 0 && oponent > 0)) trip = 0; // reset if necesary
            if (oponent > 0) trip++;
            else trip--;
            if (trip > 3 || trip < -3) flagTrip = true;
        }
        if (actual != i) localDistance += distances[actual][i];
        if (flagTrip) localDistance = localDistance*1.5;

        distance += localDistance;
        // cout << "[DEBUG] distancia hasta punto " << i << " " << distance << endl;
    }
    cout << "Distancia con Penalty: " << distance << endl;
}
