#if !defined(TabuList)
#define TabuList

#include <iostream>
#include <vector>
#include "../Objective.cpp"

using namespace std;

/**
 * struct to save the solution and the list
 * for SwapMatchRounds and SwapMatches
*/
struct SwapSolutions{
    vector<vector<int>> solution;
    vector<int> movements;
};

template <class T>
class TabuTail{
private:
  vector<T> tail;
  unsigned int maxLength;
public:
  TabuTail(int n){
    maxLength = n;
  }

  ~TabuTail(){
  }

  void addElement(T element){
    if(tail.size() == maxLength) tail.erase(tail.begin());

    tail.push_back(element);
  }

  bool InTabuTail(bool (*condition)(T, T), T auxiliar){
    for(T element : tail)
      if(condition(element, auxiliar))
        return true;
    return false;
  }

  void print(void (*printElement)(T)){
    cout << "Cantidad de elementos: " << tail.size() << endl;
    for(T element : tail)
      printElement(element);
  }
};

template <class T>
vector<vector<int>> TabuSearchOneSwap(vector<vector<int>> distances, 
                                      TSTournament scheduling, 
                                      T (*BestSwap)(vector<vector<int>>, TSTournament &, TabuTail<T>, int, int), 
                                      int iterations, 
                                      int lenList,
                                      int weight, 
                                      int DEBUG=0){
  TabuTail<T> tabuList = TabuTail<T>(lenList);
  T tempTabu;
  unsigned long int bestValue = scheduling.getDistance();

  for(int i = 0; i < iterations; i++){
    tempTabu = BestSwap(distances, scheduling, tabuList, weight, 0);
    if(bestValue > scheduling.getDistance()){
      tabuList.addElement(tempTabu);
      bestValue = scheduling.getDistance();
    }
    if(DEBUG) scheduling.print();
  }

  if(DEBUG) ObjectiveFunction(distances, scheduling.getSchedule(), weight, 0);

  return scheduling.getSchedule();
}

template <class A, class B>
vector<vector<int>> TabuSearchTwoSwaps(vector<vector<int>> distances, 
                                TSTournament scheduling, 
                                A (*BestSwapA)(vector<vector<int>>, TSTournament &, TabuTail<A>, int, int), 
                                B (*BestSwapB)(vector<vector<int>>, TSTournament &, TabuTail<B>, int, int), 
                                int iterations, 
                                int lenList,
                                int weight, 
                                int DEBUG=0){
    TabuTail<A> tabuListA = TabuTail<A>(lenList);
    TabuTail<B> tabuListB = TabuTail<B>(lenList);
    A tabuA;
    B tabuB;
    TSTournament tempA = scheduling;
    TSTournament tempB = scheduling;
    TSTournament tempBest = scheduling;

    for(int i = 0; i < iterations; i++){
        tabuA = BestSwapA(distances, tempA, tabuListA, weight, 0);
        tabuB = BestSwapB(distances, tempB, tabuListB, weight, 0);
        // check if one of the two get good results
        if(tempA.getDistance() < tempBest.getDistance() || tempB.getDistance() < tempBest.getDistance()){
            if(tempA.getDistance() < tempB.getDistance()){
                tempBest = tempA;
                tempB = tempA;
                tabuListA.addElement(tabuA);
            } else{
                tempBest = tempB;
                tempA = tempB;
                tabuListB.addElement(tabuB);
            }
        } else {
          // cout << weight << endl;
          weight = weight - weight*0.2;
        }
        if (DEBUG) tempBest.print();
    }

    if(DEBUG) ObjectiveFunction(distances, tempBest.getSchedule(), weight, 0);

    tempBest.print();
    return tempBest.getSchedule();
}

#endif