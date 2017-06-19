#include <iostream>
#include <string>
#include "queryProcessor.hpp"

using namespace std;

int main(){
  cout << "\n\nDigite sua consulta, ou \":quit\" para sair: ";
  string consulta;
  getline(cin,consulta); //lê a consulta
  queryProcessor qp;
  while (consulta.compare(":quit") != 0){
    qp.process(consulta); //processa a consulta
    cout << "\n\nDigite sua consulta, ou \":quit\" para sair.: ";
    getline(cin,consulta);
  }
  return 0;
}