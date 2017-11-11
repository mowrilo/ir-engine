#include "runAux.hpp"

runAux::runAux(int first1,int second1){
  first = first1;
  second = second1;
}

bool runAux::operator<(const runAux& a) const{
  return second > a.second;
}
