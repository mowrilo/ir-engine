#include "vocabulary.hpp"

using namespace std;

void vocabulary::addTerm(string term){
  vocab.insert({term, nid});
}

int getTermID(string term){

}
