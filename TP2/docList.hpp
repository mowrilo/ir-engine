#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <fstream>

// #define DOCSPATH "docs/"

using namespace std;

class documentList{

public:
  void addLength(int doc, int leng);
  void addUrl(int doc, string url);
  void addEdge(int doc, string edge);
  void addAnchor(string doc, string anchor);
  // void writeLen();
  // void writeUrls();
  // void writePR();
  // void writeAnchor();
  void writeAll();

private:
  unordered_map<int,int> lengths;
  unordered_map<int,string> urls;
  unordered_map<int,vector<string> > pageRankGraph;
  unordered_map<string,vector<string> > anchorTerms;
  // int docNum;
};
