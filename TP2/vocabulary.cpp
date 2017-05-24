#include "vocabulary.hpp"

using namespace std;

vocabulary::vocabulary(){
  idMax = 0;
}

void vocabulary::addTerm(string term){
  vocab.insert({term, nid});
}

int getTermID(string term){

}
