#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <utility>

// #define DOCSPATH "docs/"
#define DFACT 0.85

using namespace std;

class documentList{

public:
  documentList();
  void addLength(int doc, int leng);
  void addUrl(string url);
  // void addDomain(string url);
  void addEdge(string docUrl, vector<string> edge, vector<int> edNums);
  void addAnchor(string doc, string anchor);
  int hashFunc(string name, vector<int> weights);
  int getDocId(string name);
  string getUrlDomain(string url);
  // int isBr(string url);
  // void writeLen();
  // void writeUrls();
  // void writePR();
  // void writeAnchor();
  void writeDomPR();
  // double calcPPRValue(int doc);
  // double calcDPRValue(int dom);
  void calcPagePR();
  void calcDomPR();
  // void calcFilePR(string path);

private:
  // unordered_map<int,int> lengths;
  vector<int> pageWeights;
  vector<int> domainWeights;
  unordered_map<int,int> pages; //chave -> id do documento
  unordered_map<int,int> domains;
  unordered_map<int, unordered_set<int> > domainPageRank;
  unordered_map<int, unordered_set<int> > pagePageRank;
  unordered_map<int,int> fowardPages;
  unordered_map<int,int> fowardDoms;
  map<int, int> lengths;
  // unordered_map<int,vector<string> > pageRankGraph;
  // unordered_map<string,vector<string> > anchorTerms;
  int docNum;
  int domNum;
};
