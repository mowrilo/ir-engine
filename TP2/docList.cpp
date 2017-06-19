#include "docList.hpp"

using namespace std;
//
// documentList::documentList(){
//   docNum = 1;
// }

void documentList::addLength(int doc, int leng){
  unordered_map<int,int>::iterator it = lengths.find(doc);
  if (it == lengths.end()){
    pair<int,int> insere(doc,leng);
    lengths.insert(insere);
  }
}

void documentList::addUrl(int doc, string url){
  unordered_map<int,string>::iterator it = urls.find(doc);
  if (it == urls.end()){
    pair<int,string> insere(doc,url);
    urls.insert(insere);
  }
}

void documentList::addEdge(int doc, string edge){
  unordered_map<int,vector<string>>::iterator it = pageRankGraph.find(doc);
  if (it == pageRankGraph.end()){
    vector<string> edges;
    edges.push_back(edge);
    pair<int,vector<string> > ins(doc,edges);
    pageRankGraph.insert(ins);
  }
  else{
    it->second.push_back(edge);
  }
}

void documentList::addAnchor(string term, string doc){
  unordered_map<string,vector<string> >::iterator it = anchorTerms.find(term);
  if (it == anchorTerms.end()){
    //map<string,int> anchors;
    vector<string> vec;
    vec.push_back(doc);
    pair<string,vector<string> > par(term,vec);
    anchorTerms.insert(par);
    // pair<int,map<string,int> > ins(doc,anchors);
    // pageRankGraph.insert(ins);
  }
  else{
    it->second.push_back(doc);
    // map<string,int>::iterator it2 = it->second.find(anchor);
    // if (it2 == it->second.end()){
    //   pair<string,int> bar(anchor,1);
    //   it->second.insert(bar);
    // }
    // else{
    //   it2.second++;
    // }
  }
}

void documentList::writeAll(){
  ofstream len,url,pr,at;
  len.open("docs/lengths", ofstream::out | ofstream::app);
  url.open("docs/urls", ofstream::out | ofstream::app);
  pr.open("docs/pagerank", ofstream::out | ofstream::app);
  at.open("docs/anchortext", ofstream::out | ofstream::app);

  // len << "asd";
  for (unordered_map<int,int>::iterator it = lengths.begin(); it != lengths.end(); it++){
    len << it->first << " " << it->second << "\n";
  }
  //
  for (unordered_map<int,string>::iterator it=urls.begin(); it!=urls.end(); it++){
    url << it->first << " " << it->second << "\n";
  }
  //
  for (unordered_map<int,vector<string> >::iterator it=pageRankGraph.begin(); it!=pageRankGraph.end(); it++){
    pr << it->first;
    vector<string> aux = it->second;
    for (vector<string>::iterator iit=aux.begin(); iit != aux.end(); iit++){
      pr << " " << *iit;
    }
    pr << "\n";
  }

  for (unordered_map<string,vector<string> >::iterator it=anchorTerms.begin(); it!=anchorTerms.end(); it++){
    at << it->first;
    vector<string> aux = it->second;
    for (vector<string>::iterator iit=aux.begin(); iit != aux.end(); iit++){
      at << " " << *iit;
    }
    at << "\n";
  }

  len.close();
  url.close();
  pr.close();
  at.close();
}
