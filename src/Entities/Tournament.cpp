#include <vector>
#include <iostream>

using namespace std;

struct Game {
    int local;
    int visit;
};

struct GreedyTournament {
    vector<int> lastRival; // negative is because the rival is visit
    vector<int> trip; // positive = local, negative = visit
    vector<vector<int>> schedule; // a solution
};

void showGreedyTournament(GreedyTournament tournament) {
    cout << "--------------------------" << endl;
    cout << "Ultimos recorridos" << endl;
    for (auto &&i : tournament.trip) cout << i << " ";
    cout << endl;

    cout << "Ultimo rival" << endl;
    for (auto &&i : tournament.lastRival) cout << i << " ";
    cout << endl;

    cout << "Temporal Schedule: \n";
    // first show the name of the team
    for(long unsigned int i = 0; i < tournament.schedule[0].size(); i++)
        cout << "T" << (1+i) << " ";
    cout << endl;

    // print the matrix values
    for (vector<int> &line : tournament.schedule) {
        for(int &value : line)
            cout << ((value >= 0) ? " " : "") << value << " ";
        cout << endl;   
    }
    cout << "--------------------------" << endl;
}

class TSTournament{
private:
    vector<vector<int>> schedule; // the actual solution
public:
    TSTournament(/* args */);
    ~TSTournament();
};

TSTournament::TSTournament(/* args */){
}

TSTournament::~TSTournament(){
}
