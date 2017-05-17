#include <iostream>
#include <unordered_map>
#include <htmlcxx/html/ParserDom.h>

using namespace std;
using namespace htmlcxx;

class indexer{
  public:

  private:
    unordered_map<string, int> vocabulary;
    unordered_map<string, int> documents;
}

class fileReader{

}

class parser{

}

class tuple{
  public:
    bool operator<(const tuple& a) const;

  private:
    int term;
    int doc;
    int freq;
}
