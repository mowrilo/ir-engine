#include <iostream>
#include <queue>
#include <vector>
#include "url.hpp"
using namespace std;

int main(){
  auto compare = [](url a, url b) { return (a.getSize()) > (b.getSize());};
  priority_queue<url, vector<url>, decltype(compare)> pq(compare);
  string globo = "www.globo.com/";
  string globoesporte = "www.globo.com/esporte/";
  url url1(globo);
  url url2(globoesporte);
  pq.push(url1);
  pq.push(url2);
  while (!pq.empty()){
    url u = pq.top();
    cout << u.getSize() << " ";
    pq.pop();
   }
  return 0;
}
