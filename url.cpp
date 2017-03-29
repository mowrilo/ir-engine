#include "url.hpp"
#include <string>
#include <iostream>
using namespace std;

url::url(string &nm){
  name = nm;
  setSize(nm);
}

bool url::operator<(const url& a) const{
  return size > a.size;
}

int url::getSize(){
  return size;
}

string url::getName(){
  return name;
}

// string url:noHttp(string &name){
//   if (strncmp("http://",name,7) == 0){
//     return name.substr(7);
//   }
//   return name;
// }

void url::setSize(string &name){
  size = 0;
  for (char c: name){
    if (c == '.') size++;
    if (c == '/') size++;
  }
}
//
// bool url::isBr(){
//
// }
