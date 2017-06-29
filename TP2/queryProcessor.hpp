#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "parser.hpp"
#include "run.hpp"

#define NTHREADS 4

using namespace std;

typedef struct documentoWei{
  // double norm;
  string url;
  double score;
  // vector<double> weights;
}documentoWei;

class queryProcessor{
public:
  queryProcessor();
  string findDoc(int docId); //encontra o nome do documento
  void process(string query); //processa a consulta. método principal
  vector<int> getDocs(int termId); //lê o índice invertido e retorna os documentos de dado termo
  int getTermID(string term); //encontra a id do termo da consulta
  vector<int> mergeTerms(vector<int> term1, vector<int> term2); //interseção de duas listas
  vector<int> getDocTerms(int ndoc);
  double getWd(int ndoc, unordered_set<int> cons);
  double getPageRank(int doc);
  int getNDocs(int nterm);
  int getDomainN(int ndoc);
  int getAnchorNum(string termToFind);
  unordered_map<int,double> getAnchorFreq(vector<string> termos);
  void loadDocWeights();
  void loadPageRank();

private:
  int ndoctot;
  unordered_map<int,double> pageRanks;
  unordered_map<int,double> docWeights;
};
