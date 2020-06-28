#if !defined(Auxiliar)
#define Auxiliar

#include<vector>

using namespace std;

struct TabuSwapWithList{
    int auxA;
    int auxB;
    vector<int> list;
};

bool SwapHomesCondition(TabuSwapWithList inList, int auxA, int auxB, int auxC){
  bool cond2 = (inList.auxA == auxB && inList.auxB == auxA);
  bool cond1 = (inList.auxA == auxA && inList.auxB == auxB);

  if(cond1 || cond2)
    for(int element : inList.list)
      if(element == auxC)
        return true;

  return false;
}

#endif