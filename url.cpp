#include "url.hpp"
#include <string>
#include <iostream>
using namespace std;

url::url(string &nm){
  name = noHttp(nm);
  setSize(nm);
}

int url::getSize(){
  return size;
}

string url:noHttp(string &name){
  if (strncmp("http://",name,7) == 0){
    return name.substr(7);
  }
  return name;
}

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
