#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct Game {
    int local;
    int visit;
};

class TSTournament{
private:
    vector<vector<int>> schedule; // the actual solution
    int DEBUG; // debug option
public:
    TSTournament(int n, int debug);
    ~TSTournament();
    vector<vector<int>> getSchedule();
};

TSTournament::TSTournament(int n, int debug){
    schedule = vector<vector<int>>(2*(n - 1), vector<int>(n));
    DEBUG = debug;

    // create a list of the teams and make a shuffle
    vector<int> teams(n);
    for(int i = 0; i < n; i++)
        teams[i] = i + 1;

    random_shuffle(teams.begin(), teams.end());
    if(DEBUG) for (auto &&i : teams) cout << i << endl;

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
