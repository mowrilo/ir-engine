#include "docList.hpp"

using namespace std;
//
documentList::documentList(){
  docNum = 1;
  domNum = 1;
  // vector<int> pageWeights;
  // vector<int> domainWeights;
  for (int i=0; i<300; i++){
    pageWeights.push_back(rand()%500);
    domainWeights.push_back(rand()%500);
  }
}

string documentList::getUrlDomain(string url){
  int posFim=0, nbarras=0;
  while (nbarras < 3){ //Vai até a posição da terceira barra, considerando que as duas primeiras estão em "http://"
    if (url[posFim] == '/')  nbarras++;
    posFim++;
  }
  // int posBr = isBr(url);//Se for .br, a posição de referência passa a ser o .br
  // int i=posFim-1;
  // int j=posFim-1;
  // if (posBr > 0){
  //   i = posBr;
  //   j = posBr;
  // }
  // i--;
  // while ((url[i] != '.') && (i>0)) i--; //Anda-se para trás, até o próximo ponto '.'
  // if (i > url.length()) return ""; //Retorna URL vazia para evitar exceções de limites da string
  // string subUrl = url.substr(i,(j-i)); //Pega a próxima parte do domínio
  // bool isdmn = isDomain(subUrl);        //e checa se faz parte dos padrões de domínio
  // if (isdmn){
  //   i--;
  //   while ((url[i] != '.') && (url[i] != '/')) i--; //Vai até o próximo ponto ou barra
  // }
  // i++;
  // if (i > url.length()) return "";//Retorna URL vazia para evitar exceções de limites da string
  return url.substr(0,posFim); //Retorna o domínio
}

void documentList::addLength(int doc, int leng){
  ofstream len;
  len.open("docs/len", ios::out | ios::app);
  len << doc << " " << leng << "\n";
  len.close();
  // unordered_map<int,int>::iterator it = lengths.find(doc);
  // pair<int,int> insere(doc,leng);
  // lengths.insert(insere);
  // if (it == lengths.end()){
  // }
}

void documentList::addUrl(string url){
  string domain = getUrlDomain(url);
  // cout << "weights size: " << pageWeights.size() <<"\n";
  int keyPg = hashFunc(url, pageWeights);
  int keyDom = hashFunc(domain, domainWeights);
  // cout << "oioi1.5\n";
  unordered_map<int,int>::iterator itPg = pages.find(keyPg);
  if (itPg == pages.end()){
    int num = docNum;
    docNum++;
    pair<int,int> adic(keyPg, num);
    pages.insert(adic);
    ofstream arq;
    arq.open("docs/urls", ios::out | ios::app);
    arq << num << " " << url << "\n";
    arq.close();
  }

  unordered_map<int,int>::iterator itDom = domains.find(keyDom);
  if (itDom == domains.end()){
    int num = domNum;
    domNum++;
    pair<int,int> adic(keyDom, num);
    domains.insert(adic);
    ofstream arq;
    arq.open("docs/domains", ios::out | ios::app);
    arq << num << " " << domain << "\n";
    arq.close();
  }
  // unordered_map<int,string>::iterator it = urls.find(doc);
  // if (it == urls.end()){
  //   pair<int,string> insere(doc,url);
  //   urls.insert(insere);
  // }
}

int documentList::getDocId(string name){
  int keyPg = hashFunc(name, pageWeights);
  unordered_map<int,int>::iterator it = pages.find(keyPg);
  if (it == pages.end())  return -1;
  return it->second;
}

void documentList::addEdge(string docUrl, vector<string> edges, vector<int> edNums){
  ofstream arq;//, arqDom;
  arq.open("docs/pagerankPg", ios::out | ios::app);
  // arqDom.open("docs/pagerankDom", ios::out | ios::app);
  string domain = getUrlDomain(docUrl);
  int keyPg = hashFunc(docUrl, pageWeights);
  int keyDom = hashFunc(domain, domainWeights);
  unordered_map<int,int>::iterator it = pages.find(keyPg);
  int pgNum = it->second;
  unordered_map<int,int>::iterator itDom = domains.find(keyDom);
  int domnNum = itDom->second;
  unordered_map<int, unordered_set<int> >::iterator itPR = domainPageRank.find(domnNum);
  if (itPR == domainPageRank.end()){
    unordered_set<int> usAux;
    pair<int,unordered_set<int> > aux(domnNum,usAux);
    domainPageRank.insert(aux);
  }
  itPR = domainPageRank.find(domnNum);
  // cout << "size of edges: " << edges.size() <<"\n";
  for (int i=0; i<edges.size(); i++){
    string domEdg = getUrlDomain(edges[i]);
    int keyDomEdg = hashFunc(domEdg, domainWeights);
    unordered_map<int,int>::iterator itDomEdg = domains.find(keyDomEdg);
    // cout << "dominio: " << domEdg << "size: " << domEdg.size() << "\n";
    int domnNumEdg = itDomEdg->second;
    // cout << "morreu =(\n";
    unordered_set<int>::iterator itEdg = itPR->second.find(domnNumEdg);
    if (itEdg == itPR->second.end()){
      itPR->second.insert(domnNumEdg);
    }
  }

  arq << pgNum;
  for (vector<int>::iterator itNum=edNums.begin(); itNum!=edNums.end();  ++itNum){
    arq << " " << *itNum;
  }
  arq << "\n";
  arq.close();
  // unordered_map<int,vector<string>>::iterator it = pageRankGraph.find(doc);
  // if (it == pageRankGraph.end()){
  //   vector<string> edges;
  //   edges.push_back(edge);
  //   pair<int,vector<string> > ins(doc,edges);
  //   pageRankGraph.insert(ins);
  // }
  // else{
  //   it->second.push_back(edge);
  // }
}

// int documentList::isBr(string url){
//   string br = ".br/"; //Checa se existe o padrão ".br" na URL
//   int find = url.find(br);
//   if (find == string::npos) find=-1;
//   return find;
// }

int documentList::hashFunc(string name, vector<int> weights){
  int n=0;
  int i=0;
  // cout << "weights size: " << weights.size() << "\n";
  for (char c: name){
    n+=c*weights[i];
    // cout << "oi daqui de dentro\n";
    i++;
  }
  return n;
}

// void documentList::addAnchor(string term, string doc){
//   unordered_map<string,vector<string> >::iterator it = anchorTerms.find(term);
//   if (it == anchorTerms.end()){
//     //map<string,int> anchors;
//     vector<string> vec;
//     vec.push_back(doc);
//     pair<string,vector<string> > par(term,vec);
//     anchorTerms.insert(par);
//     // pair<int,map<string,int> > ins(doc,anchors);
//     // pageRankGraph.insert(ins);
//   }
//   else{
//     it->second.push_back(doc);
//     // map<string,int>::iterator it2 = it->second.find(anchor);
//     // if (it2 == it->second.end()){
//     //   pair<string,int> bar(anchor,1);
//     //   it->second.insert(bar);
//     // }
//     // else{
//     //   it2.second++;
//     // }
//   }
// }

void documentList::writeDomPR(){
  ofstream arq;
  arq.open("docs/pagerankDom", ios::out | ios::app);
  for (unordered_map<int, unordered_set<int> >::iterator it = domainPageRank.begin(); it != domainPageRank.end(); it++){
    int domOrig = it->first;
    unordered_set<int> domDests = it->second;
    arq << domOrig;
    for (unordered_set<int>::iterator iit = domDests.begin(); iit != domDests.end(); iit++){
      arq << " " << *iit;
    }
    arq << "\n";
  }
}

// void documentList::writeAll(){
//   ofstream len,url,pr,at;
//   len.open("docs/lengths", ofstream::out | ofstream::app);
//   url.open("docs/urls", ofstream::out | ofstream::app);
//   pr.open("docs/pagerank", ofstream::out | ofstream::app);
//   at.open("docs/anchortext", ofstream::out | ofstream::app);
//
//   // len << "asd";
//   for (unordered_map<int,int>::iterator it = lengths.begin(); it != lengths.end(); it++){
//     len << it->first << " " << it->second << "\n";
//   }
//   //
//   for (unordered_map<int,string>::iterator it=urls.begin(); it!=urls.end(); it++){
//     url << it->first << " " << it->second << "\n";
//   }
//   //
//   for (unordered_map<int,vector<string> >::iterator it=pageRankGraph.begin(); it!=pageRankGraph.end(); it++){
//     pr << it->first;
//     vector<string> aux = it->second;
//     for (vector<string>::iterator iit=aux.begin(); iit != aux.end(); iit++){
//       pr << " " << *iit;
//     }
//     pr << "\n";
//   }
//
//   for (unordered_map<string,vector<string> >::iterator it=anchorTerms.begin(); it!=anchorTerms.end(); it++){
//     at << it->first;
//     vector<string> aux = it->second;
//     for (vector<string>::iterator iit=aux.begin(); iit != aux.end(); iit++){
//       at << " " << *iit;
//     }
//     at << "\n";
//   }
//
//   len.close();
//   url.close();
//   pr.close();
//   at.close();
// }
