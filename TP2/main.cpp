#include <iostream>
#include "fileReader.hpp"
#include "parser.hpp"

#define NTHREADS 7
#define PATH_HTMLS "/home/murilo/Documentos/ri/TP2/"//"/media/murilo/hdext/collection/"

using namespace std;

int main(){
  //shared_ptr(new fileReader);
  fileReader fr(PATH_HTMLS, 0, NTHREADS);
  parser ps;
  for (int i=0; i<1; i++){
    fr.openNextFile();
    string test = fr.getNextHtml();
    // cout << test;
    ps.parse(test);
    fr.closeFile();
  }
  return 0;
}
