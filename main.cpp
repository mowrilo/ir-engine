#include <iostream>
#include "url.hpp"
using namespace std;

int main(){
  string globo = "www.globo.com/";
  url url1(globo);
  cout << url1.getSize();
  return 0;
}
