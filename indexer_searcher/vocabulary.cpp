#include "vocabulary.hpp"

using namespace std;

vocabulary::vocabulary(string name){
  idMax = 1; //ID disponível para a próxima inserção
  nameOfFile = name;
}

void vocabulary::addTerm(string term){
  map<string, int>::iterator it = vocab.find(term);
  if (it == vocab.end()){ //caso não tenha o termo, adiciona e escreve em arquivo
    pair<string, int> entrada(term,idMax);
    vocab.insert(entrada);
    ofstream file;
    string printFile = nameOfFile + term[0];
    file.open(printFile, ios::out | ios::app);
    file << term << " " << idMax << "\n";
    file.close();
    idMax++;
  }
  // else{
  // }
}

void vocabulary::print(){
  for (map<string,int>::iterator it=vocab.begin(); it!=vocab.end(); it++){
    ofstream file;
    string printFile = nameOfFile + it->first[0];
    file.open(nameOfFile, ios::out); //escreve o vocabulario em arquivo
    file << vocab.size() << "\n";
    file << it->first << "  " << it->second << "\n";
    file.close();
  }
  cout << "Final vocabulary size: " << vocab.size() << "\n";
}

int vocabulary::size(){
  return vocab.size();
}

int vocabulary::getTermID(string term){ //retorna o numero de identificação do termo
  map<string, int>::iterator it = vocab.find(term);
  if (it != vocab.end()){
    return it->second;
  }
  return -1;
}
