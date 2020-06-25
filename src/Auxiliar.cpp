#include <iostream>
#include <vector>

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