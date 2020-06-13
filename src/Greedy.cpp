#include <iostream>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <algorithm>
#include "./Entities/Tournament.cpp"

using namespace std;

int total;
/**
 * Select random rival to the first team
 * in the first date
 * also is random who is local and who is visit
*/
Game startPoint() {
    Game game;
    int opponent = (rand()%(total-1))+2;
    // select local or visit
    if ((rand()%2)) {
        game.local = 1;
        game.visit = opponent;
    } else {
        game.local = opponent;
        game.visit = 1;
    }
    return game;
}

/**
 * add teams to the list and eliminate from free
*/
void addToSolution(int row, Game game, GreedyTournament &tournament, vector<int> &freeTeams) {
    int local = game.local - 1;
    int visit = game.visit -1;
    // add to tournament
    tournament.schedule[row][local] = game.visit * -1;
    tournament.schedule[row][visit] = game.local;
    // "eliminate" from free teams
    freeTeams[local] = -1;
    freeTeams[visit] = -1;
    // update lastRival list
    tournament.lastRival[local] = game.visit * -1;
    tournament.lastRival[visit] = game.local;
    // update trip list
    // if local is in a visit trip reset, else add 1
    if (tournament.trip[local] < 0) tournament.trip[local] = 1;
    else tournament.trip[local]++; 
    // if visit is in a home stand reset, else rest 1
    if (tournament.trip[visit] > 0) tournament.trip[visit] = -1;
    else tournament.trip[visit]--; 
}

/**
 * Function that determinate if the team can be a rival
 * @team: the real value of the team - 1
 * @rival: the value of a rival
 * @alreadyPlayed: teams that the team already played
 * @side: local=1 or visit=-1
 * @lastGame: the number of the lastRival - 1
*/
bool canPlay(int team, int rival, vector<int> alreadyPlayed, int lastGame){
    // cout << "[DEBUG] values in canPlay()\n";
    // cout << "team: " << team << ", rival: " << rival << ", lastGame: " << lastGame << endl;

    // iterators to validate a team
    vector<int>::iterator itrBegin = alreadyPlayed.begin();
    vector<int>::iterator itrEnd = alreadyPlayed.end();
    // cout << "[DEBUG] already played: \n";
    // for (auto &&i : alreadyPlayed) cout << i << " ";
    // cout << endl;
    // some pre conditions
    bool isNotLastGame = abs(lastGame) != rival;
    bool isFree = rival != -1;
    bool preCondition = isNotLastGame && isFree && team != rival;
    bool findCondition = find(itrBegin, itrEnd, rival) == itrEnd && (find(itrBegin, itrEnd, -1*rival) == itrEnd);
    // the condition, the find function is the final part
    bool condition = preCondition && findCondition;
    return condition;
}

/**
 * insert rivals asuming the team is local
*/
void playLocal(vector<int> freeTeams, vector<int> alreadyPlayed, int team, int lastGame, vector<int> &priorityValues, vector<int> &priorityList, int value=0) {
    for(int &city : freeTeams) { // add the other freeTeams
        if (canPlay(team + 1, city, alreadyPlayed, lastGame)) {
            priorityValues.insert(priorityValues.begin(), value);
            priorityList.insert(priorityList.begin(), -1 * city);
        }
    }
}

/**
 * insert rivals asuming the team is visit
*/
void playVisit(vector<int> freeTeams, vector<int> alreadyPlayed, int team, int lastGame, vector<int> &priorityValues, vector<int> &priorityList, vector<vector<int>> distances, int location = -1) {
    if (location == -1) location = team; // point to calculate distances
    // find rival considering team is in other city
    for(long unsigned int i = 0; i < distances[location].size(); i++) {
        // check if the team is free (not playing in this row or already played versus this team)
        // and is not the actual team
        // cout << "foreign team " << freeTeams[i] << endl;
        if (freeTeams[i] != (team + 1) && canPlay(team + 1, freeTeams[i], alreadyPlayed, lastGame)) {
            // add team to the list in order
            vector<int>::iterator itrValue;
            vector<int>::iterator itrList = priorityList.begin();
            for (itrValue = priorityValues.begin(); itrValue != priorityValues.end(); itrValue++) {
                itrList++;
                if (distances[location][i] < *itrValue) break;
            }
            // cout << " [DEBUG] add team " << freeTeams[i] << endl;
            priorityList.insert(itrList, freeTeams[i]);
            priorityValues.insert(itrValue, distances[location][i]);
        }
    }
}

/**
 * from a team, see the near teams and select the option that accept: 
 * -) a team cant play more than 3 games in a row local or visit
 * -) if a team is visit and the next game is visit,
 * the distance is between this two cities
 * -) cant play versus the same team in two continuos rows
*/
Game selectRival(int team, vector<int> freeTeams, vector<vector<int>> distances, GreedyTournament &tournament){
    Game game;
    int lastGame = tournament.lastRival[team];
    vector<int> alreadyPlayed;
    vector<int> priorityList; // list of posible movements
    vector<int> priorityValues; // list of value of the movement
    vector<vector<int>> schedule = tournament.schedule;

    // select the rivals that i already have
    for (long unsigned int i = 0; i < schedule.size() && schedule[i][team] != 0; i++)
        alreadyPlayed.push_back(schedule[i][team]);

    // cout << "[DEBUG] Team " << team + 1 << " already play versus:\n";  
    // for (auto &&i : alreadyPlayed) cout << i << " ";
    // cout << endl;
    
    // team can continue being visit
    if (lastGame > 0 && tournament.trip[team] > -3) {
        int postLastGame = abs(lastGame) - 1;
        // find rival considering team is in other city
        playVisit(freeTeams, alreadyPlayed, team, lastGame, priorityValues, priorityList, distances, postLastGame);
        
        // cout << "[DEBUG] condition can play visit" << endl;
        // for (auto &&i : priorityList) cout << i << " ";
        // cout << endl;
        // for (auto &&i : priorityValues) cout << i << " ";
        // cout << endl;

        // condition doesn't have more teams
        if (priorityList.size() == 0) {
            int value = distances[postLastGame][team];
            // give lastGame the original form
            // save the near cities
            playLocal(freeTeams, alreadyPlayed, team, lastGame, priorityValues, priorityList, value);
            // cout << "[DEBUG] condition have to play local" << endl;
            // for (auto &&i : priorityList) cout << i << " ";
            // cout << endl;
            // for (auto &&i : priorityValues) cout << i << " ";
            // cout << endl;
        }
    }

    // team can play continue being local
    if (lastGame <= 0 && tournament.trip[team] < 3) {
        playLocal(freeTeams, alreadyPlayed, team, lastGame, priorityValues, priorityList);

        // cout << "[DEBUG] condition can play local" << endl;
        // for (auto &&i : priorityList) cout << i << " ";
        // cout << endl;
        // for (auto &&i : priorityValues) cout << i << " ";
        // cout << endl;
        // condition doesn't have more teams
        if (priorityList.size() == 0) {
            // the team have to play as visit
            playVisit(freeTeams, alreadyPlayed, team, lastGame, priorityValues, priorityList, distances);

            // cout << "[DEBUG] condition have to play as visit" << endl;
            // for (auto &&i : priorityList) cout << i << " ";
            // cout << endl;
            // for (auto &&i : priorityValues) cout << i << " ";
            // cout << endl;
        }
    }
    
    // team have to play as local when the last trip was been visit
    if (lastGame > 0 && tournament.trip[team] <= -3) {
        // save distance from the foreign city to our city
        int value = distances[abs(lastGame)-1][team];
        // save the near cities
        playLocal(freeTeams, alreadyPlayed, team, lastGame, priorityValues, priorityList, value);

        // cout << "[DEBUG] condition have to play as local" << endl;
        // for (auto &&i : priorityList) cout << i << " ";
        // cout << endl;
        // for (auto &&i : priorityValues) cout << i << " ";
        // cout << endl;
    }

    // team have to play as visit when the last trip was been local
    if (lastGame < 0 && tournament.trip[team] >= 3) {
        playVisit(freeTeams, alreadyPlayed, team, lastGame, priorityValues, priorityList, distances);

        // cout << "[DEBUG] condition have to play as visit" << endl;
        // for (auto &&i : priorityList) cout << i << " ";
        // cout << endl;
        // for (auto &&i : priorityValues) cout << i << " ";
        // cout << endl;
    }
    
    bool addFlag = false;
    // add rival
    for (auto &&rival : priorityList){
        // TODO check if the iteration is posible
        if (rival > 0){
            if (tournament.trip[rival-1] < 3) {
                game.local = rival;
                game.visit = team + 1;
                addFlag = true;
                break;
            }
            // if is not posible we check the next value
        } else {
            if (tournament.trip[abs(rival)-1] > -3) {
                game.local = team + 1;
                game.visit = abs(rival);
                addFlag = true;
                break;
            }
        }
    }
    if (!addFlag) { // didnt add a team
        int rival = priorityList[0]; // add the first element in the list
        if (rival > 0){
            game.local = rival;
            game.visit = team + 1;
        } else {
            game.local = team + 1;
            game.visit = abs(rival);
        } 
    }
    

    return game;
}

/**
 * Auxiliar function to make recursion
*/
void NGreedy(int row, GreedyTournament &tournament, vector<vector<int>> distances){
    if (row == (int)tournament.schedule.size() / 2) return; // stop condition
    // just a auxiliar game
    Game game;
    // list of not selected teams
    vector<int> freeTeams(total); // -1 = team is selected
    iota(freeTeams.begin(), freeTeams.end(), 1);

    // 1.- Iteration for the first row
    if (row == 0){
        // define start point
        game = startPoint();
        // add teams to solution, and eliminate from free teams
        addToSolution(row, game, tournament, freeTeams);
    }

    // add games to the row
    for (long unsigned int i = 0; i < freeTeams.size(); i++) { // move between free teams
        if (freeTeams[i] != -1) { // a team that is not selected in this row
            game = selectRival(i, freeTeams, distances, tournament);
            addToSolution(row, game, tournament, freeTeams);
            // showGreedyTournament(tournament);
        }
    }

    // move to the next row
    NGreedy(row+1, tournament, distances);
}

/**
 * generate the mirror of a scheduling
*/
void mirror(vector<vector<int>> &schedule) {
    long unsigned int half = schedule.size() / 2;
    for (long unsigned int i = 0; i < half; i++)
        for (long unsigned int j = 0; (int)j < total; j++)
            schedule[i+half][j] = -1 * schedule[i][j];
}

vector<vector<int>> Greedy(vector<vector<int>> distances){
    total = distances.size();
    GreedyTournament tournament;
    // solution is going to be a matrix
    tournament.schedule = vector<vector<int>>(2*(total - 1), vector<int>(total));
    tournament.trip = vector<int>(total);
    tournament.lastRival = vector<int>(total);

    NGreedy(0, tournament, distances);

    // generate mirror
    mirror(tournament.schedule);
    return tournament.schedule;
}