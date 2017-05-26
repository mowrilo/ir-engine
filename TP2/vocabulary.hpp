#include <iostream>
#include <unordered_map>
#include <utility>
#include <fstream>

using namespace std;

class vocabulary{
  public:
    vocabulary();
    void addTerm(string term);
    int getTermID(string term);
    void print();
    int size();

  private:
    //mutex voc;
    unordered_map<string, int> vocab;
    int idMax;
};
