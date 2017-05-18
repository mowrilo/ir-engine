#include "vocabulary.hpp"

using namespace std;

vocabulary::vocabulary(){
  idMax = 0;
}

void vocabulary::addTerm(string term){
  vocab.insert({term, nid});
}

int getTermID(string term){
  unordered_map<string,int>::iterator it = vocab.find(term);
  if (it == vocab.end())  return -1;
  return it->second;
}
