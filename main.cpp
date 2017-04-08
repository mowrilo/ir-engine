#include <iostream>
#include <queue>
#include <vector>
#include <CkSpider.h>
#include <thread>
#include "crawler.hpp"
#include "kmp.hpp"
using namespace std;

//FAZER HASH PARA DOMINIOS DIFERENTES


int main(){
  string path = "./htmls";
  crawler cl(path);
  cl.begin();
  //string text = "http://google.com/";//"http://mundoeducacao.bol.uol.com.br/biologia/origem-vida.htm";
  //string pattern = ".br/";
  //cout << "\n\nFAFSDF   " << text.substr(13,4) << endl;
  //vector<string> a = cl.domainTypes;
  //cout << a.size();
  //string domain = cl.getUrlDomain(text);
  //cout << "domain = " << domain << endl;
  // for (int i=0; i<9; i++){
  //   cout << a[i];
  // }
  // cout << "ei\n";
  // string a = cl.getUrlDomain(text);
  //bool a = cl.isBr(text);
  //int pos = find(pattern, text);
  // cout << "ei\n";
  // if ((pos+3) == text.length()) cout << "aew" << endl;

  // cout << "pos= " << pos << endl;
  //if (a)	cout << "ISBR" << endl;
  //else		cout << "ISNOTBR" << endl;
  return 0;
}
