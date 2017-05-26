#include "triple.hpp"

triple::triple(int termNum,int docNum,int freqNum, int runNumber){
  nterm=termNum;
  ndoc=docNum;
  freq=freqNum;
  nrun=runNumber;
}

bool triple::operator<(const triple& a) const{
  if (nterm == a.nterm) return ndoc < a.ndoc;
  return nterm < a.nterm;
  //else if (ndoc != a.ndoc)
  //return freq < a.freq;
}
