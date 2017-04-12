#include "url.hpp"
#include <string>
#include <iostream>
using namespace std;

url::url(string &nm, string &dm, int offset){
  name = nm;
  domain = dm;
  weight = setWeight(nm,offset);
  setValid(nm);
}

void url::setValid(string &name){
  isValid = (name.size() > 0);
}

bool url::checkValid(){
  return isValid;
}

bool url::operator<(const url& a) const{
  return weight > a.weight;
}

int url::getWeight(){
  return weight;
}

string url::getName(){
  return name;
}

string url::getDomain(){
  return domain;
}
void url::increaseWeight(int i){
  weight+=i;
}

int url::setWeight(string &name, int offset){
  int peso = 0;
  for (char c: name){
    if (c == '.') peso++;
    if (c == '/') peso++;
  }
  peso*=10;
  peso+=offset;
  return peso;
}
