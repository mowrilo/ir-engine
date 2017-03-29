#include "url.hpp"
#include <iostream>
using namespace std;

url::url(string &nm){
  name = nm;
  setSize(nm);
}

int url::getSize(){
  return size;
}

void url::setSize(string &name){
size = 0;
  for (char c: name){
    if (c == '.') size++;
    if (c == '/') size++;
  }
}
