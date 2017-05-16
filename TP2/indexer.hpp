#include <iostream>
#include <unordered_map>
#include <htmlcxx/html/ParserDom.h>

using namespace std;
using namespace htmlcxx;

class indexer {
  public:

  private:
    unordered_map<string, int> vocabulary;
    unordered_map<string, int> documents;
}

class tuple {

  private:
    int term;
    int doc;
    int freq;
    vector<int> pos;
}
