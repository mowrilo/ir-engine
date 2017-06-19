#include "vocabulary.hpp"

using namespace std;

vocabulary::vocabulary(){
  idMax = 1; //ID disponível para a próxima inserção
}

void vocabulary::addTerm(string term){
  map<string, int>::iterator it = vocab.find(term);
  if (it == vocab.end()){ //caso não tenha o termo, adiciona e escreve em arquivo
    pair<string, int> entrada(term,idMax);
    vocab.insert(entrada);
    ofstream file;
    file.open("vocabulary", ios::app);
    file << idMax << " " << term << "\n";
    file.close();
    idMax++;
  }
  else{
  }
}

void vocabulary::print(){
  ofstream file;
  file.open("vocabulary", ios::out); //escreve o vocabulario em arquivo
  file << vocab.size() << "\n";
  for (map<string,int>::iterator it=vocab.begin(); it!=vocab.end(); it++){
    file << it->first << "  " << it->second << "\n";
  }
  file.close();
  cout << "voc size: " << vocab.size() << "\n";
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
