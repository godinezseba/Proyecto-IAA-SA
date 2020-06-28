#if !defined(TabuList)
#define TabuList

#include <iostream>
#include <vector>

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
};

#endif