#include <iostream>
#include <queue>
#include <vector>
#include <CkSpider.h>
#include <thread>
#include "crawler.hpp"
using namespace std;

//FAZER HASH PARA DOMINIOS DIFERENTES


int main(){
  string path = "/media/murilo/storage/pathHtmls";
  crawler cl(path);
  //cout << "OI";
  cl.begin();
  return 0;
}
