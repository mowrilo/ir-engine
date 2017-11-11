#include <iostream>
#include <unordered_map>
#include <map>
#include <utility>
#include <fstream>

using namespace std;

class vocabulary{
  public:
    vocabulary(string name); //construtor
    void addTerm(string term); //adiciona um termo, caso não exista
    int getTermID(string term); //retorna o numero de identificação
    void print(); //escreve em um arquivo (não utilizada)
    int size(); //retorna o tamanho do vocabulário

  private:
    map<string, int> vocab;
    int idMax;
    string nameOfFile;
};
