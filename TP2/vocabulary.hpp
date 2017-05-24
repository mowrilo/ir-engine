#include <iostream>
#include <unordered_map>
#include <utility>

using namespace std;

class vocabulary{
  public:
    vocabulary();
    void addTerm(string term);
    int getTermID(string term);
    void print();

  private:
    //mutex voc;
    unordered_map<string, int> vocab;
    int idMax;
};
