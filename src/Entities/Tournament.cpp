#if !defined(Tournament)
#define Tournament

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

/**
 * function to show a solution with the format
*/
void showSolution(vector<vector<int>> solution) {
    cout << "Schedule: \n";
    // first show the name of the team
    cout << "R\\T|";
    for(long unsigned int i = 0; i < solution[0].size(); i++)
        cout << "T" << i+1 << ((i+1 >= 10) ? " " : "  ");
    cout << endl;

    // print the matrix values
    int round = 0;
    for (vector<int> &line : solution) {
        ++round;
        cout << " " << round << ((round >= 10) ? "" : " ") <<"|";
        for(int &value : line)
            cout << (value >= 0 ? " " : "") << value << (abs(value) < 10 ? "  " : " ");
        cout << endl;   
    }
}

class TSTournament{
private:
    vector<vector<int>> schedule; // the actual solution
    unsigned long int distance; // the objective result
    int DEBUG; // debug option
public:
    TSTournament();
    TSTournament(int n, int debug);
    ~TSTournament();
    vector<vector<int>> getSchedule();
    void setSchedule(vector<vector<int>> newSchedule);
    unsigned long int getDistance();
    void setDistance(unsigned long int d);
    void print();
};

TSTournament::TSTournament(){
    distance = 0;
    DEBUG = 0;
}

TSTournament::TSTournament(int n, int debug){
    schedule = vector<vector<int>>(2*(n - 1), vector<int>(n));
    DEBUG = debug;

    // create a list of the teams and make a shuffle
    vector<int> teams(n);
    for(int i = 0; i < n; i++)
        teams[i] = i + 1;

    random_shuffle(teams.begin(), teams.end());

    // use 1-factorization logic
    int lastTeam = teams[n-1]; // last team is always the same
    vector<int> auxTeams(teams.begin(), teams.end()-1); // use to rotate
    // first half
    for(long unsigned int round = 0; round < schedule.size()/2; round++){
        for (int i = 1; i < n/2; i++){
            schedule[round][auxTeams[i]-1] = -auxTeams[n-1-i];
            schedule[round][auxTeams[n-1-i]-1] = auxTeams[i];
        }
        schedule[round][auxTeams[0]-1] = -lastTeam;
        schedule[round][lastTeam-1] = auxTeams[0];
        // move the teams one to the left
        rotate(auxTeams.begin(), auxTeams.begin()+1, auxTeams.end());
    }
    // second half
    for(long unsigned int round = schedule.size()/2; round < schedule.size(); round++){
        for (int i = 1; i < n/2; i++){
            schedule[round][auxTeams[i]-1] = auxTeams[n-1-i];
            schedule[round][auxTeams[n-1-i]-1] = -auxTeams[i];
        }
        schedule[round][auxTeams[0]-1] = lastTeam;
        schedule[round][lastTeam-1] = -auxTeams[0];
        // move the teams one to the left
        rotate(auxTeams.begin(), auxTeams.begin()+1, auxTeams.end());
    }
}

TSTournament::~TSTournament(){
}

vector<vector<int>> TSTournament::getSchedule(){
    return schedule;
}

void TSTournament::setSchedule(vector<vector<int>> newSchedule){
    schedule = newSchedule;
}

unsigned long int TSTournament::getDistance(){
    return distance;
}

void TSTournament::setDistance(unsigned long int d){
    distance = d;
}

void TSTournament::print(){
    if(DEBUG) cout << "=======================================================" << endl;
    cout << /*"Distancia: " << */ distance << endl;
    // showSolution(this->getSchedule()); // comment this to only get value
    if(DEBUG) cout << "=======================================================" << endl;
}

#endif // Tournament
