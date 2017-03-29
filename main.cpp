#include <iostream>
#include <queue>
#include <vector>
#include <CkSpider.h>
#include "crawler.hpp"
using namespace std;

int main(){
  crawler cl;
  // scheduler sch;
  // CkSpider spider;
  // string globo = "www.globo.com.br/";
  // string gag = "www.9gag.com/";
  // string globoesporte = "http://www.globo.com.br/esporte/index.html";
  // string globoesportea = spider.canonicalizeUrl(globoesporte.c_str());
  // cout << globoesportea;
  // url url1(globo);
  // url url2(globoesportea);
  // url url3(gag);
  // sch.addInbound(url1);
  // sch.addInbound(url2);
  // sch.addInbound(url3);
  // int i=3;
  // while (i>0){
  //   url u = sch.getInbound();
  //   cout << u.getName() << "\n";
  //   i--;
  //  }
  cl.begin();
  return 0;
}
