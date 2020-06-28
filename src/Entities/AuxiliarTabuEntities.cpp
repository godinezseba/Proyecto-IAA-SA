#if !defined(Auxiliar)
#define Auxiliar

#include <iostream>
#include <vector>

using namespace std;

struct TabuSwapWithList{
    int auxA;
    int auxB;
    vector<int> list;
};

void printAuxiliar(TabuSwapWithList element){
  cout << "auxA: " << element.auxA << ", auxB: " << element.auxB << endl;
  for(int element : element.list)
    cout << element << ", ";
  cout << endl;
}

bool AuxCondition(TabuSwapWithList inList, TabuSwapWithList element){
  bool cond2 = (inList.auxA == element.auxB && inList.auxB == element.auxA);
  bool cond1 = (inList.auxA == element.auxA && inList.auxB == element.auxB);

  int auxC = element.list[0];

  if(cond1 || cond2)
    for(int e : inList.list)
      if(e == auxC)
        return true;

  return false;
}

#endif