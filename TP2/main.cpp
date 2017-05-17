#include <iostream>
#include "fileReader.hpp"

#define NTHREADS 7
#define PATH_HTMLS "/media/murilo/hdext/collection/"

using namespace std;

int main(){
  fileReader fr(PATH_HTMLS, 0, NTHREADS);
  for (int i=0; i<10; i++){
    fr.openNextFile();
    string test = fr.getNextHtml();
    // cout << test;
    fr.closeFile();
  }
  return 0;
}
