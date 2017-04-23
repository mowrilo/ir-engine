#include <iostream>
#include <queue>
#include <vector>
#include <CkSpider.h>
#include <thread>
#include "crawler.hpp"
using namespace std;

int main(){
  string path = "./htmls";
  crawler cl(path); //Instancia o crawler
  cl.begin(); //Inicia o processo de coleta
  return 0;
}
