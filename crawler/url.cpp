#include "url.hpp"
#include <string>
#include <iostream>
using namespace std;

url::url(string &nm, string &dm, int offset){ //Construtor
  name = nm;
  domain = dm;
  weight = setWeight(nm,offset); //Define o peso inicial
  setValid(nm); //Define a validade
}

void url::setValid(string &name){
  isValid = (name.size() > 0); //Caso não esteja vazia, é válida
}

bool url::checkValid(){ //Retorna a validade
  return isValid;
}

bool url::operator<(const url& a) const{
  return weight > a.weight; //Operador baseado nos pesos, para a comparação na fila
}

int url::getWeight(){ //Retorna o peso
  return weight;
}

string url::getName(){ //Retorna o nome
  return name;
}

string url::getDomain(){ //Retorna o dominio
  return domain;
}
void url::increaseWeight(int i){ //Multiplica por i e incrementa o peso, pois o overflow da multiplicação é
  weight*=i;                      //lidado como módulo (%). Icrementar evita peso zero.
  weight++;
}

int url::setWeight(string &name, int offset){
  int peso = 0;
  for (char c: name){ //Conta o número de pontos e barras.
    if (c == '.') peso++;
    if (c == '/') peso++;
  }
  peso*=17; //Peso incial é múltiplo do número de pontos e barras, mais o offset.
  peso+=offset;
  return peso;
}
