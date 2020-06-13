#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "./Greedy.cpp"
#include "./Auxiliar.cpp"

using namespace std;

int n;
int seed;
int DEBUG;

int readInput(int argc, char **argv){ 
    seed = atoi(argv[1]);
    n = atoi(argv[2]);
    DEBUG = atoi(argv[3]);

    return 1;
}

void calculateDistance(vector<vector<int>> solution, vector<vector<int>> distances) {
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
        // cout << "[DEBUG] distancia hasta punto " << i << " " << distance << endl;
    }
    cout << "Distancia total: " << distance << endl;
}

void calculateDistancePenalty(vector<vector<int>> solution, vector<vector<int>> distances) {
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

int main(int argc, char *argv[]){
    readInput(argc, argv);

    vector<vector<int>> distances(n, vector<int>(n));
    vector<vector<int>> solution;

    // read input matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> distances[i][j];
        }
    }

    // define seed
    srand(seed);

    // give matrix to the algorithm
    solution = Greedy(distances);
    
    //show solution
    calculateDistance(solution, distances);
    calculateDistancePenalty(solution, distances);
    showSolution(solution);
    return 0;
}
