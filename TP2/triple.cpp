#include "triple.hpp"

triple::triple(int termNum,int docNum,int freqNum){
  nterm=termNum;
  ndoc=docNum;
  freqNum=freq;
}

bool triple::operator<(const triple& a) const{
  if (nterm != a.nterm) return nterm < a.nterm;
  else if (ndoc != a.ndoc) return ndoc < a.ndoc;
  return freq < a.freq;
}
