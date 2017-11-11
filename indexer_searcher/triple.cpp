#include "triple.hpp"

triple::triple(int termNum,int docNum,int freqNum, int runNumber){
  nterm=termNum; //inicializa atributos
  ndoc=docNum;
  freq=freqNum;
  nrun=runNumber;
}

bool triple::operator<(const triple& a) const{ //comparador
  if (nterm == a.nterm) return ndoc > a.ndoc;
  return nterm > a.nterm;
}
