#include "vocabulary.hpp"

using namespace std;

vocabulary::vocabulary(){
  idMax = 1;
}

void vocabulary::addTerm(string term){
  // cout << "Checando termo " << term << "...\n";
  unordered_map<string, int>::iterator it = vocab.find(term);
  if (it == vocab.end()){
    if (idMax==1) cout << "termo 1: " << term << "\n";
    // cout << "Termo não existe no vocabulário! Adicionando...\n";
    pair<string, int> entrada(term,idMax);
    vocab.insert(entrada);
    idMax++;
  }
  else{
    // cout << "Termo existe no vocabulário e tem id " << it->second << "\n";
  }
}

void vocabulary::print(){
  ofstream file;
  file.open("vocabulary", ios::out);
  for (unordered_map<string,int>::iterator it=vocab.begin(); it!=vocab.end(); it++){
    file << it->second << "  " << it->first << "\n";
  }
  file.close();
  cout << "voc size: " << vocab.size() << "\n";
}

int vocabulary::size(){
  return vocab.size();
}

int vocabulary::getTermID(string term){
  unordered_map<string, int>::iterator it = vocab.find(term);
  if (it != vocab.end()){
    return it->second;
  }
  return -1;
}
