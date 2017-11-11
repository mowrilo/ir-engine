#include <iostream>

class triple{
  public:
    triple(int termNum,int docNum,int freqNum,int runNumber); //construtor
    bool operator<(const triple& a) const; //comparador para a fila de prioridades
    int nterm; //atributos da tripla para comparação
    int ndoc;
    int freq;
    int nrun;
};
