#include <iostream>
#include <vector>

using namespace std;

/**
 * function to show a solution with the format
*/
void showSolution(vector<vector<int>> solution) {
    cout << "Schedule: \n";
    // first show the name of the team
    for(long unsigned int i = 0; i < solution[0].size(); i++)
        cout << "T" << (1+i) << " ";
    cout << endl;

    // print the matrix values
    for (vector<int> &line : solution) {
        for(int &value : line)
            cout << ((value >= 0) ? " " : "") << value << " ";
        cout << endl;   
    }
}