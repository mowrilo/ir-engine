#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "parser.hpp"
#include "run.hpp"

using namespace std;

class queryProcessor{
public:
  queryProcessor();
  string findDoc(int docId); //encontra o nome do documento
  void process(string query); //processa a consulta. método principal
  vector<int> getDocs(string term); //lê o índice invertido e retorna os documentos de dado termo
  int getTermID(string term); //encontra a id do termo da consulta
  vector<int> mergeTerms(vector<int> term1, vector<int> term2); //interseção de duas listas
};
