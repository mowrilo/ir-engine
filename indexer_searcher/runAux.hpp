#include <iostream>

class runAux{
  public:
    runAux(int first1,int second1);
    bool operator<(const runAux& a) const;
    int first;
    int second;
};
