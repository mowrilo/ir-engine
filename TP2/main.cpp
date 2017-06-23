#include <iostream>
#include <fstream>
#include <algorithm>
#include "indexer.hpp"
#include "eliasCoding.hpp"
#include <ctime>

#define PATH_HTMLS "/media/murilo/hdext/collection/collection/"

using namespace std;

int main(){
  indexer id;
  id.start(PATH_HTMLS); //começa a indexação
  // eliasCoding::encodeAndWrite(1196, 15, 2,"asd",true);
  // run a("./runs/run0");
  // // while (!a.isEOF()){
  //   vector<int> dec = a.readAndDecode();
  //   cout << dec.size() << " ";
  //   for (vector<int>::iterator it=dec.begin(); it != dec.end(); it++){
  //     cout << *it << " ";
  //   }
  //   cout << "\n";
  // }

  // run b("./runs/run2");
  // while (!b.isEOF()){
  //
  //   vector<int> dec = b.readAndDecode();
  //   // if (dec.size() != 3) cout << dec.size() << " ";
  //   // for (vector<int>::iterator it=dec.begin(); it != dec.end(); it++){
  //   //   cout << *it << " ";
  //   // }
  //   // cout << "\n";
  // }

  // run c("./runs/run2");
  // // while (!a.isEOF()){
  //   dec = c.readAndDecode();
  //   cout << dec.size() << " ";
  //   for (vector<int>::iterator it=dec.begin(); it != dec.end(); it++){
  //     cout << *it << " ";
  //   }
  //   cout << "\n";
  // }

  // run d("./runs/run3");
  // // while (!a.isEOF()){
  //   dec = d.readAndDecode();
  //   cout << dec.size() << " ";
  //   for (vector<int>::iterator it=dec.begin(); it != dec.end(); it++){
  //     cout << *it << " ";
  //   }
  //   cout << "\n";
  // }
  // sortBlock sb(10, "anchorRuns/run");
  // sb.buildIndex(4);
  // sb.sortAll(4);
  return 0;
}
