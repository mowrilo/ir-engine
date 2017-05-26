#include <iostream>

class triple{
  public:
    triple(int termNum,int docNum,int freqNum,int runNumber);
    bool operator<(const triple& a) const;
    int nterm;
    int ndoc;
    int freq;
    int nrun;
};
