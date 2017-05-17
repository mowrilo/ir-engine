#include <iostream>
#include <unordered_map>
#include <htmlcxx/html/ParserDom.h>

using namespace std;
using namespace htmlcxx;

class index{
  public:
    void addTerm
    void addDocument
    string getTerm
    string getDocument

  private:
    mutex voc
    mutex doc
    unordered_map<string, int> vocabulary;
    unordered_map<string, int> documents;
}

class indexer{
  public:
    void index(string path);
    void start(strint path);

  private:
    string path_to_files;
}


class parser{
  public:
    string getTerm;

  private:
    string html;
}

class tuple{
  public:
    bool operator<(const tuple& a) const;

  private:
    int term;
    int doc;
    int freq;
}
